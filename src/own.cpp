
#include "precompiled.hpp"
#include "own.hpp"
#include "err.hpp"
#include "io_thread.hpp"

zmq::own_t::own_t (class ctx_t *parent_, uint32_t tid_) :
    object_t (parent_, tid_),
    _terminating (false),
    _sent_seqnum (0),
    _processed_seqnum (0),
    _owner (NULL),
    _term_acks (0)
{
}

zmq::own_t::own_t (io_thread_t *io_thread_, const options_t &options_) :
    object_t (io_thread_),
    options (options_),
    _terminating (false),
    _sent_seqnum (0),
    _processed_seqnum (0),
    _owner (NULL),
    _term_acks (0)
{
}

zmq::own_t::~own_t ()
{
}

void zmq::own_t::set_owner (own_t *owner_)
{
    zmq_assert (!_owner);
    _owner = owner_;
}

void zmq::own_t::inc_seqnum ()
{
    //  This function may be called from a different thread!
    //  这个函数可以在不同的线程中调用
    _sent_seqnum.add (1);
}

void zmq::own_t::process_seqnum ()
{
    //  Catch up with counter of processed commands.
    //  累加已经处理命令的计数器
    _processed_seqnum++;

    //  We may have catched up and still have pending terms acks.
    //  我们也许已经达到相同的数字或还有暂存的terms acks
    check_term_acks ();
}

void zmq::own_t::launch_child (own_t *object_)
{
    //  Specify the owner of the object.
    //  将此对象设置为 输入对象的 拥有者
    object_->set_owner (this);

    //  Plug the object into the I/O thread.
    //  将输入的对象插入到一个IO线程中
    send_plug (object_);

    //  Take ownership of the object.
    //  接管输入对象，此对象将负责输入对象的析构
    send_own (this, object_);
}

void zmq::own_t::term_child (own_t *object_)
{
    process_term_req (object_);
}

void zmq::own_t::process_term_req (own_t *object_)
{
    //  When shutting down we can ignore termination requests from owned
    //  objects. The termination request was already sent to the object.
    //  当正在处于终止之中，说明终止的请求已经发送到了 要求终止的对象上了， 直接返回
    if (_terminating)
        return;

    //  If not found, we assume that termination request was already sent to
    //  the object so we can safely ignore the request.
    //  如果没有找到这个终止的对象， 我们权当已经发送了终止请求到这个对象上面，所以可以忽略此请求
    if (0 == _owned.erase (object_))
        return;

    //  If I/O object is well and alive let's ask it to terminate.
    //  如果IO对象是存在的，并且还或者， 就让他自己终止吧
    register_term_acks (1);

    //  Note that this object is the root of the (partial shutdown) thus, its
    //  value of linger is used, rather than the value stored by the children.
    //  注意到， 如果这个是根对象可能存在部分关闭的情况，因此使用的是此对象的linger值，而不是子对象的linger值
    send_term (object_, options.linger.load ());
}

void zmq::own_t::process_own (own_t *object_)
{
    //  If the object is already being shut down, new owned objects are
    //  immediately asked to terminate. Note that linger is set to zero.
    //  如果当前的对象是正在进行关闭，新对象会立刻要求终止，linger设置成0
    if (_terminating) {
        //到这里， object 已经有owner了
        register_term_acks (1);
        send_term (object_, 0);
        return;
    }

    //  Store the reference to the owned object.
    //  存到当前的集合中
    _owned.insert (object_);
}

// 要求拥有者终止自己
void zmq::own_t::terminate ()
{
    //  If termination is already underway, there's no point
    //  in starting it anew.
    //  如果终止已经在进行了， 就不重新发起了
    if (_terminating)
        return;

    //  As for the root of the ownership tree, there's no one to terminate it,
    //  so it has to terminate itself.
    //  如果没有父节点，那就自己终止自己吧
    if (!_owner) {
        process_term (options.linger.load ());
        return;
    }

    //  If I am an owned object, I'll ask my owner to terminate me.
    //  如果我是被拥有的对象， 请求拥有者结束自己
    send_term_req (_owner, this);
}

bool zmq::own_t::is_terminating () const
{
    return _terminating;
}

void zmq::own_t::process_term (int linger_)
{
    //  Double termination should never happen.
    //  两次终止是不可能的
    zmq_assert (!_terminating);

    //  Send termination request to all owned objects.
    //  发送终止命令给所有的被拥有的对象
    for (owned_t::iterator it = _owned.begin (), end = _owned.end (); it != end; ++it)
        send_term (*it, linger_);
    // 调整预计收到term acks 的数量
    register_term_acks (static_cast<int> (_owned.size ()));
    // 清空集合
    _owned.clear ();

    //  Start termination process and check whether by chance we cannot
    //  terminate immediately.
    //  设置终止标志，并且检查是否可以立刻停止
    _terminating = true;
    check_term_acks ();
}

void zmq::own_t::register_term_acks (int count_)
{
    _term_acks += count_;
}

void zmq::own_t::unregister_term_ack ()
{
    zmq_assert (_term_acks > 0);
    _term_acks--;

    //  This may be a last ack we are waiting for before termination...
    //  这个可能是最后一个我们正在等待的ack
    check_term_acks ();
}

void zmq::own_t::process_term_ack ()
{
    unregister_term_ack ();
}

void zmq::own_t::check_term_acks ()
{
    if (_terminating && _processed_seqnum == _sent_seqnum.get ()
        && _term_acks == 0) {
        //  Sanity check. There should be no active children at this point.
        //  一致性检查。这个时候应该没有任何一个活动的子对象
        zmq_assert (_owned.empty ());

        //  The root object has nobody to confirm the termination to.
        //  Other nodes will confirm the termination to the owner.
        //  如果这个对象是被别的对象拥有的话，需要发送term ack 到它的拥有者
        if (_owner)
            send_term_ack (_owner);

        //  Deallocate the resources.
        //  析构回收资源
        process_destroy ();
    }
}

void zmq::own_t::process_destroy ()
{
    delete this;
}
