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

#ifndef __ZMQ_SOCKET_BASE_HPP_INCLUDED__
#define __ZMQ_SOCKET_BASE_HPP_INCLUDED__

#include <string>
#include <map>
#include <stdarg.h>

#include "own.hpp"
#include "array.hpp"
#include "blob.hpp"
#include "stdint.hpp"
#include "poller.hpp"
#include "i_poll_events.hpp"
#include "i_mailbox.hpp"
#include "clock.hpp"
#include "pipe.hpp"
#include "endpoint.hpp"

extern "C" {
void zmq_free_event (void *data_, void *hint_);
}

namespace zmq
{
class ctx_t;
class msg_t;
class pipe_t;

class socket_base_t : public own_t,
                      public array_item_t<>,
                      public i_poll_events,
                      public i_pipe_events
{
    friend class reaper_t;

  public:
    //  Returns false if object is not a socket.
    //  如果是返回假的话，这个对象不是socket
    bool check_tag () const;

    //  Returns whether the socket is thread-safe.
    //  返回这个对象是否是线程安全的
    bool is_thread_safe () const;

    //  Create a socket of a specified type.
    //  给定对象类型，创建指定的socket
    static socket_base_t *
    create (int type_, zmq::ctx_t *parent_, uint32_t tid_, int sid_);

    //  Returns the mailbox associated with this socket.
    //  返回这个对象的邮箱
    i_mailbox *get_mailbox () const;

    //  Interrupt blocking call if the socket is stuck in one.
    //  如果这个socket被卡住了， 可以调用这个函数来打断
    //  This function can be called from a different thread!
    //  扎个函数可以在不同线程中调用
    void stop ();

    //  Interface for communication with the API layer.
    //  与API层通信的接口
    int setsockopt (int option_, const void *optval_, size_t optvallen_);
    int getsockopt (int option_, void *optval_, size_t *optvallen_);
    int bind (const char *endpoint_uri_);
    int connect (const char *endpoint_uri_);
    int term_endpoint (const char *endpoint_uri_);
    int send (zmq::msg_t *msg_, int flags_);
    int recv (zmq::msg_t *msg_, int flags_);
    void add_signaler (signaler_t *s_);
    void remove_signaler (signaler_t *s_);
    int close ();

    //  These functions are used by the polling mechanism to determine
    //  which events are to be reported from this socket.
    //  轮训机制使用这些函数来确定将从该套接字报告那些事件
    bool has_in ();
    bool has_out ();

    //  Joining and leaving groups
    //  加入和离开组
    int join (const char *group_);
    int leave (const char *group_);

    //  Using this function reaper thread ask the socket to register with
    //  its poller.
    //  reaper 线程使用这个函数将这个socket加入reaper 的poller
    void start_reaping (poller_t *poller_);

    //  i_poll_events implementation. This interface is used when socket
    //  is handled by the poller in the reaper thread.
    //  poll_event接口的实现。 reaper 线程内执行相关事件的处理函数
    void in_event () ZMQ_FINAL;
    void out_event () ZMQ_FINAL;
    void timer_event (int id_) ZMQ_FINAL;

    //  i_pipe_events interface implementation.
    //  pipe_events 接口的实现
    void read_activated (pipe_t *pipe_) ZMQ_FINAL;
    void write_activated (pipe_t *pipe_) ZMQ_FINAL;
    void hiccuped (pipe_t *pipe_) ZMQ_FINAL;
    void pipe_terminated (pipe_t *pipe_) ZMQ_FINAL;
    void lock ();
    void unlock ();

    int monitor (const char *endpoint_,
                 uint64_t events_,
                 int event_version_,
                 int type_);

    void event_connected (const endpoint_uri_pair_t &endpoint_uri_pair_,
                          zmq::fd_t fd_);
    void event_connect_delayed (const endpoint_uri_pair_t &endpoint_uri_pair_,
                                int err_);
    void event_connect_retried (const endpoint_uri_pair_t &endpoint_uri_pair_,
                                int interval_);
    void event_listening (const endpoint_uri_pair_t &endpoint_uri_pair_,
                          zmq::fd_t fd_);
    void event_bind_failed (const endpoint_uri_pair_t &endpoint_uri_pair_,
                            int err_);
    void event_accepted (const endpoint_uri_pair_t &endpoint_uri_pair_,
                         zmq::fd_t fd_);
    void event_accept_failed (const endpoint_uri_pair_t &endpoint_uri_pair_,
                              int err_);
    void event_closed (const endpoint_uri_pair_t &endpoint_uri_pair_,
                       zmq::fd_t fd_);
    void event_close_failed (const endpoint_uri_pair_t &endpoint_uri_pair_,
                             int err_);
    void event_disconnected (const endpoint_uri_pair_t &endpoint_uri_pair_,
                             zmq::fd_t fd_);
    void event_handshake_failed_no_detail (
      const endpoint_uri_pair_t &endpoint_uri_pair_, int err_);
    void event_handshake_failed_protocol (
      const endpoint_uri_pair_t &endpoint_uri_pair_, int err_);
    void
    event_handshake_failed_auth (const endpoint_uri_pair_t &endpoint_uri_pair_,
                                 int err_);
    void
    event_handshake_succeeded (const endpoint_uri_pair_t &endpoint_uri_pair_,
                               int err_);

    //  Query the state of a specific peer. The default implementation
    //  always returns an ENOTSUP error.
    //  查询给定对端的状态，默认的实现是返回ENOTSUP错误，只有ROUTER类型才使用这个
    virtual int get_peer_state (const void *routing_id_,
                                size_t routing_id_size_) const;

    //  Request for pipes statistics - will generate a ZMQ_EVENT_PIPES_STATS
    //  after gathering the data asynchronously. Requires event monitoring to
    //  be enabled.
    //  管道统计请求，异步收集数据完成后，将会产生一个ZMQ_EVENT_PIPES_STATS事件。需要有监控的事件
    int query_pipes_stats ();

    bool is_disconnected () const;

  protected:
    socket_base_t (zmq::ctx_t *parent_,
                   uint32_t tid_,
                   int sid_,
                   bool thread_safe_ = false);
    ~socket_base_t () ZMQ_OVERRIDE;

    //  Concrete algorithms for the x- methods are to be defined by
    //  individual socket types.
    //  每个socket类型，要实现自己的x- 方法
    virtual void xattach_pipe (zmq::pipe_t *pipe_,
                               bool subscribe_to_all_ = false,
                               bool locally_initiated_ = false) = 0;

    //  The default implementation assumes there are no specific socket
    //  options for the particular socket type. If not so, ZMQ_FINAL this
    //  method.
    //  每一种socket类型，默认的实现假设没有具体的socket选项, 如果不是这样，就用ZMQ_FINAL标识此方法
    virtual int
    xsetsockopt (int option_, const void *optval_, size_t optvallen_);

    //  The default implementation assumes that send is not supported.
    //  默认的实现假设不支持发送功能
    virtual bool xhas_out ();
    virtual int xsend (zmq::msg_t *msg_);

    //  The default implementation assumes that recv in not supported.
    //  默认的额实现假设不支持接收功能
    virtual bool xhas_in ();
    virtual int xrecv (zmq::msg_t *msg_);

    //  i_pipe_events will be forwarded to these functions.
    //  i_pipe_events 将会转到这些函数处理
    virtual void xread_activated (pipe_t *pipe_);
    virtual void xwrite_activated (pipe_t *pipe_);
    virtual void xhiccuped (pipe_t *pipe_);
    virtual void xpipe_terminated (pipe_t *pipe_) = 0;

    //  the default implementation assumes that joub and leave are not supported.
    //  默认的实现是袈裟不支持join和leave功能
    virtual int xjoin (const char *group_);
    virtual int xleave (const char *group_);

    //  Delay actual destruction of the socket.
    //  socket延迟销毁
    void process_destroy () ZMQ_FINAL;

    int connect_internal (const char *endpoint_uri_);

    // Mutex for synchronize access to the socket in thread safe mode
    // 在线程安全模式用于同步进入这个socket
    mutex_t _sync;

  private:
    // test if event should be sent and then dispatch it
    // 测试事件是否被发送和转发
    void event (const endpoint_uri_pair_t &endpoint_uri_pair_,
                uint64_t values_[],
                uint64_t values_count_,
                uint64_t type_);

    // Socket event data dispatch
    // socket事件转发
    void monitor_event (uint64_t event_,
                        const uint64_t values_[],
                        uint64_t values_count_,
                        const endpoint_uri_pair_t &endpoint_uri_pair_) const;

    // Monitor socket cleanup
    // 清理监控socket
    void stop_monitor (bool send_monitor_stopped_event_ = true);

    //  Creates new endpoint ID and adds the endpoint to the map.
    //  创建新的节点ID和添加到节点的map
    void add_endpoint (const endpoint_uri_pair_t &endpoint_pair_,
                       own_t *endpoint_,
                       pipe_t *pipe_);

    //  Map of open endpoints.
    //  节点的map
    typedef std::pair<own_t *, pipe_t *> endpoint_pipe_t;
    typedef std::multimap<std::string, endpoint_pipe_t> endpoints_t;
    endpoints_t _endpoints;

    //  Map of open inproc endpoints.
    //  进程内部通信节点map
    class inprocs_t
    {
      public:
        void emplace (const char *endpoint_uri_, pipe_t *pipe_);
        int erase_pipes (const std::string &endpoint_uri_str_);
        void erase_pipe (const pipe_t *pipe_);

      private:
        typedef std::multimap<std::string, pipe_t *> map_t;
        map_t _inprocs;
    };
    inprocs_t _inprocs;

    //  To be called after processing commands or invoking any command
    //  handlers explicitly. If required, it will deallocate the socket.
    //  在处理命令之后或者调用任何命令处理器都要调用这个函数， 如果需要的话，他讲析构这个对象
    void check_destroy ();

    //  Moves the flags from the message to local variables,
    //  to be later retrieved by getsockopt.
    //  将消息的标识释放到本地的变量中，以备用getsockopt去做查询
    void extract_flags (const msg_t *msg_);

    //  Used to check whether the object is a socket.
    //  用于检查这个对象是否是socket
    uint32_t _tag;

    //  If true, associated context was already terminated.
    //  如果是真的话， 相关的上下文内容已经被终止了
    bool _ctx_terminated;

    //  If true, object should have been already destroyed. However,
    //  destruction is delayed while we unwind the stack to the point
    //  where it doesn't intersect the object being destroyed.
    //  如果是真的话，对象应该被销毁了，当我们的这个栈与已经销毁的对象没有相交时，这个销毁动作会被延迟
    bool _destroyed;

    //  Parse URI string.
    //  解析URI字符串
    static int
    parse_uri (const char *uri_, std::string &protocol_, std::string &path_);

    //  Check whether transport protocol, as specified in connect or
    //  bind, is available and compatible with the socket type.
    //  检查在connect还是bind中指定的传输协议是否和当前的socket 类型兼容
    int check_protocol (const std::string &protocol_) const;

    //  Register the pipe with this socket.
    //  注册pipe到这个socket中
    void attach_pipe (zmq::pipe_t *pipe_,
                      bool subscribe_to_all_ = false,
                      bool locally_initiated_ = false);

    //  Processes commands sent to this socket (if any). If timeout is -1,
    //  returns only after at least one command was processed.
    //  If throttle argument is true, commands are processed at most once
    //  in a predefined time period.
    //  处理发送到这个socket 上的命令， 如果timeout是-1的话，至少处理一条命令才返回。
    //  如果throttle参数是真的话，在这段时间内最多处理一条命令
    int process_commands (int timeout_, bool throttle_);

    //  Handlers for incoming commands.
    //  命令处理器
    void process_stop () ZMQ_FINAL;
    void process_bind (zmq::pipe_t *pipe_) ZMQ_FINAL;
    void
    process_pipe_stats_publish (uint64_t outbound_queue_count_,
                                uint64_t inbound_queue_count_,
                                endpoint_uri_pair_t *endpoint_pair_) ZMQ_FINAL;
    void process_term (int linger_) ZMQ_FINAL;
    void process_term_endpoint (std::string *endpoint_) ZMQ_FINAL;

    void update_pipe_options (int option_);

    std::string resolve_tcp_addr (std::string endpoint_uri_,
                                  const char *tcp_address_);

    //  Socket's mailbox object.
    //  socket 的邮箱对象
    i_mailbox *_mailbox;

    //  List of attached pipes.
    //  连接上的的pipe列表
    typedef array_t<pipe_t, 3> pipes_t;
    pipes_t _pipes;

    //  Reaper's poller and handle of this socket within it.
    //  reaper的轮训器和他内部是用的句柄
    poller_t *_poller;
    poller_t::handle_t _handle;

    //  Timestamp of when commands were processed the last time.
    //  处理最后一条命令的时间戳
    uint64_t _last_tsc;

    //  Number of messages received since last command processing.
    //  自上次命令处理依赖所接收消息的数量
    int _ticks;

    //  True if the last message received had MORE flag set.
    //  如果是真的话上一条消息有MORE的表示
    bool _rcvmore;

    //  Improves efficiency of time measurement.
    //  提升时间量测的效率
    clock_t _clock;

    // Monitor socket;
    // 监控socket
    void *_monitor_socket;

    // Bitmask of events being monitored
    // 正在监控事件的掩码
    int64_t _monitor_events;

    // Last socket endpoint resolved URI
    // Last socket endpoint resolved URI
    std::string _last_endpoint;

    // Indicate if the socket is thread safe
    // 线程安全标识
    const bool _thread_safe;

    // Signaler to be used in the reaping stage
    // 在回收阶段使用的信号器
    signaler_t *_reaper_signaler;

    // Mutex to synchronize access to the monitor Pair socket
    // 监控socket的同步互斥量
    mutex_t _monitor_sync;

    ZMQ_NON_COPYABLE_NOR_MOVABLE (socket_base_t)

    // Add a flag for mark disconnect action
    // 断开行为的表示
    bool _disconnected;
};

// 具有路由功能的socket 基类
class routing_socket_base_t : public socket_base_t
{
  protected:
    routing_socket_base_t (class ctx_t *parent_, uint32_t tid_, int sid_);
    ~routing_socket_base_t () ZMQ_OVERRIDE;

    // methods from socket_base_t
    int xsetsockopt (int option_,
                     const void *optval_,
                     size_t optvallen_) ZMQ_OVERRIDE;
    void xwrite_activated (pipe_t *pipe_) ZMQ_FINAL;

    // own methods
    // 自己有用的方法
    std::string extract_connect_routing_id ();
    bool connect_routing_id_is_set () const;

    struct out_pipe_t
    {
        pipe_t *pipe;
        bool active;
    };

    void add_out_pipe (blob_t routing_id_, pipe_t *pipe_);
    bool has_out_pipe (const blob_t &routing_id_) const;
    out_pipe_t *lookup_out_pipe (const blob_t &routing_id_);
    const out_pipe_t *lookup_out_pipe (const blob_t &routing_id_) const;
    void erase_out_pipe (const pipe_t *pipe_);
    out_pipe_t try_erase_out_pipe (const blob_t &routing_id_);
    template <typename Func> bool any_of_out_pipes (Func func_)
    {
        bool res = false;
        for (out_pipes_t::iterator it = _out_pipes.begin (),
                                   end = _out_pipes.end ();
             it != end && !res; ++it) {
            res |= func_ (*it->second.pipe);
        }

        return res;
    }

  private:
    //  Outbound pipes indexed by the peer IDs.
    //  用对端的ID，建立出站管道的索引
    typedef std::map<blob_t, out_pipe_t> out_pipes_t;
    out_pipes_t _out_pipes;

    // Next assigned name on a zmq_connect() call used by ROUTER and STREAM socket types
    // Router 和 Stream socket 使用zmq_connect() 调用的下一个指定个名称
    std::string _connect_routing_id;
};
}

#endif
