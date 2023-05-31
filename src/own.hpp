/*
    Copyright (c) 2007-2016 Contributors as noted in the AUTHORS file

    This file is part of libzmq, the ZeroMQ core engine in C++.

    libzmq is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    As a special exception, the Contributors give you permission to link
    this library with independent modules to produce an executable,
    regardless of the license terms of these independent modules, and to
    copy and distribute the resulting executable under terms of your choice,
    provided that you also meet, for each linked independent module, the
    terms and conditions of the license of that module. An independent
    module is a module which is not derived from or based on this library.
    If you modify this library, you must extend this exception to your
    version of the library.

    libzmq is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __ZMQ_OWN_HPP_INCLUDED__
#define __ZMQ_OWN_HPP_INCLUDED__

#include <set>

#include "object.hpp"
#include "options.hpp"
#include "atomic_counter.hpp"
#include "stdint.hpp"

namespace zmq
{
class ctx_t;
class io_thread_t;

//  Base class for objects forming a part of ownership hierarchy.
//  It handles initialisation and destruction of such objects.
//  用来形成从属关系结构的基类，它处理从属对象的初始化和销毁

class own_t : public object_t
{
  public:
    //  Note that the owner is unspecified in the constructor.
    //  It'll be supplied later on when the object is plugged in.
    //  在构造函数中没有指定所有者，在后面当对象被插入进来的时候会指定的

    //  The object is not living within an I/O thread. It has it's own
    //  thread outside of 0MQ infrastructure.
    //  使用这个函数创建的对象，不是活在IO线程中的。他是活在0MQ框架之外的自己的线程中
    //  如： socket
    own_t (zmq::ctx_t *parent_, uint32_t tid_);

    //  The object is living within I/O thread.
    //  这个对象是活在IO线程中的，如： tcp_connector_t
    own_t (zmq::io_thread_t *io_thread_, const options_t &options_);

    //  When another owned object wants to send command to this object
    //  it calls this function to let it know it should not shut down
    //  before the command is delivered.
    //  当另一个所属对象想要发送命令到此对象，它需要先调用这个inc_seqnum()函数，让此对象知道在没有接收到
    //  命令之前是不能够关闭的
    void inc_seqnum ();

    //  Use following two functions to wait for arbitrary events before
    //  terminating. Just add number of events to wait for using
    //  register_tem_acks functions. When event occurs, call
    //  remove_term_ack. When number of pending acks reaches zero
    //  object will be deallocated.
    //  在终止之前，可以使用下面这两个函数去等待任意数量的事件。使用register_term_acks 去增加需要等待事件的数量
    //  当事件发生时，调用remove_term_ack()。当暂存的acks降到0，表示对象可以被析构了
    void register_term_acks (int count_);
    void unregister_term_ack ();

  protected:
    //  Launch the supplied object and become its owner.
    //  让提供的对象运行，并且变成它的所有者
    void launch_child (own_t *object_);

    //  Terminate owned object
    //  终止所拥有的一个对象
    void term_child (own_t *object_);

    //  Ask owner object to terminate this object. It may take a while
    //  while actual termination is started. This function should not be
    //  called more than once.
    //  要求拥有者终止此对象。实际启动终止动作可能需要一点时间。这个函数不能调用超过一次
    void terminate ();

    //  Returns true if the object is in process of termination.
    //  当此对象处于终止的过程中，返回true
    bool is_terminating () const;

    //  Derived object destroys own_t. There's no point in allowing
    //  others to invoke the destructor. At the same time, it has to be
    //  virtual so that generic own_t deallocation mechanism destroys
    //  specific type of the owned object correctly.
    //   继承类对象销毁own_t， 其他的对象没有办法调用此析构函数。
    //   同时，这个函数需要是virtual的，以便能够正确的销毁所拥有的对象
    ~own_t () ZMQ_OVERRIDE;

    //  Term handler is protected rather than private so that it can
    //  be intercepted by the derived class. This is useful to add custom
    //  steps to the beginning of the termination process.
    //  终止处理是受保护类型而不是私有的，主要是因为子类可以使用这个方法，这样做的好处是
    //  可以增加定制的步骤到开始终止的流程开头，参考：stream_listener_base_t
    void process_term (int linger_) ZMQ_OVERRIDE;

    //  A place to hook in when physical destruction of the object
    //  is to be delayed.
    //  这个是回收物理内存的地方，可能会被延迟执行
    virtual void process_destroy ();

    //  Socket options associated with this object.
    //  这个对象关联的socket选项
    options_t options;

  private:
    //  Set owner of the object
    //  设置此对象的拥有者
    void set_owner (own_t *owner_);

    //  Handlers for incoming commands.
    //  处理到来的命令
    void process_own (own_t *object_) ZMQ_OVERRIDE;
    void process_term_req (own_t *object_) ZMQ_OVERRIDE;
    void process_term_ack () ZMQ_OVERRIDE;
    void process_seqnum () ZMQ_OVERRIDE;

    //  Check whether all the pending term acks were delivered.
    //  If so, deallocate this object.
    //  检查所有暂存的term acks 是否被消掉，如果全消掉，析构此对象
    void check_term_acks ();

    //  True if termination was already initiated. If so, we can destroy
    //  the object if there are no more child objects or pending term acks.
    //  如果已经发起了终止过程，则这个值为真。如果没有拥有任何对象或者暂存的term acks，我们就可以
    //  销毁此对象
    bool _terminating;

    //  Sequence number of the last command sent to this object.
    //  此对象发送命令的最后一个序列号
    atomic_counter_t _sent_seqnum;

    //  Sequence number of the last command processed by this object.
    //  此对象处理最后一条命令的序列号
    uint64_t _processed_seqnum;

    //  Socket owning this object. It's responsible for shutting down
    //  this object.
    //  拥有此对象的socket，它负责关闭此对象
    own_t *_owner;

    //  List of all objects owned by this socket. We are responsible
    //  for deallocating them before we quit.
    //  此对象所有拥有的对象们， 在此对象退出时，有责任有义务销毁拥有的对象
    typedef std::set<own_t *> owned_t;
    owned_t _owned;

    //  Number of events we have to get before we can destroy the object.
    //  在此对象销毁前， 需要获取的ack事件的数量
    int _term_acks;

    ZMQ_NON_COPYABLE_NOR_MOVABLE (own_t)
};
}

#endif
