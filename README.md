# zmq_cn_comment
中文注释zmq的源代码， 希望能更深入了解zmq的工作原理和代码写法技巧。此仅供学习使用

| file                      | done |
|---------------------------|------|
| address.cpp               | -    |
| address.hpp               | -    |
| array.hpp                 | -    |
| atomic_counter.hpp        | -    |
| atomic_ptr.hpp            | -    |
| blob.hpp                  | -    |
| channel.cpp               | -    |
| channel.hpp               | -    |
| client.cpp                | -    |
| client.hpp                | -    |
| clock.cpp                 | -    |
| clock.hpp                 | -    |
| command.hpp               | -    |
| compat.hpp                | -    |
| condition_variable.hpp    | -    |
| config.hpp                | -    |
| ctx.cpp                   | -    |
| ctx.hpp                   | -    |
| curve_client.cpp          | -    |
| curve_client.hpp          | -    |
| curve_client_tools.hpp    | -    |
| curve_mechanism_base.cpp  | -    |
| curve_mechanism_base.hpp  | -    |
| curve_server.cpp          | -    |
| curve_server.hpp          | -    |
| dbuffer.hpp               | -    |
| dealer.cpp                | -    |
| dealer.hpp                | -    |
| decoder_allocators.cpp    | -    |
| decoder_allocators.hpp    | -    |
| decoder.hpp               | -    |
| devpoll.cpp               | -    |
| devpoll.hpp               | -    |
| dgram.cpp                 | -    |
| dgram.hpp                 | -    |
| dish.cpp                  | -    |
| dish.hpp                  | -    |
| dist.cpp                  | -    |
| dist.hpp                  | -    |
| encoder.hpp               | -    |
| endpoint.cpp              | -    |
| endpoint.hpp              | -    |
| epoll.cpp                 | -    |
| epoll.hpp                 | -    |
| err.cpp                   | -    |
| err.hpp                   | -    |
| fd.hpp                    | -    |
| fq.cpp                    | -    |
| fq.hpp                    | -    |
| gather.cpp                | -    |
| gather.hpp                | -    |
| generic_mtrie.hpp         | -    |
| generic_mtrie_impl.hpp    | -    |
| gssapi_client.cpp         | -    |
| gssapi_client.hpp         | -    |
| gssapi_mechanism_base.cpp | -    |
| gssapi_mechanism_base.hpp | -    |
| gssapi_server.cpp         | -    |
| gssapi_server.hpp         | -    |
| i_decoder.hpp             | -    |
| i_encoder.hpp             | -    |
| i_engine.hpp              | -    |
| i_mailbox.hpp             | -    |
| io_object.cpp             | -    |
| io_object.hpp             | -    |
| io_thread.cpp             | -    |
| io_thread.hpp             | -    |
| ipc_address.cpp           | -    |
| ipc_address.hpp           | -    |
| ipc_connecter.cpp         | -    |
| ipc_connecter.hpp         | -    |
| ipc_listener.cpp          | -    |
| ipc_listener.hpp          | -    |
| ip.cpp                    | -    |
| ip.hpp                    | -    |
| i_poll_events.hpp         | -    |
| ip_resolver.cpp           | -    |
| ip_resolver.hpp           | -    |
| kqueue.cpp                | -    |
| kqueue.hpp                | -    |
| lb.cpp                    | -    |
| lb.hpp                    | -    |
| libzmq.pc                 | -    |
| libzmq.pc.in              | -    |
| libzmq.vers               | -    |
| likely.hpp                | -    |
| log.hpp                   | -    |
| macros.hpp                | -    |
| mailbox.cpp               | -    |
| mailbox.hpp               | -    |
| mailbox_safe.cpp          | -    |
| mailbox_safe.hpp          | -    |
| mechanism_base.cpp        | -    |
| mechanism_base.hpp        | -    |
| mechanism.cpp             | -    |
| mechanism.hpp             | -    |
| metadata.cpp              | -    |
| metadata.hpp              | -    |
| msg.cpp                   | -    |
| msg.hpp                   | -    |
| mtrie.cpp                 | -    |
| mtrie.hpp                 | -    |
| mutex.hpp                 | -    |
| norm_engine.cpp           | -    |
| norm_engine.hpp           | -    |
| null_mechanism.cpp        | -    |
| null_mechanism.hpp        | -    |
| object.cpp                | -    |
| object.hpp                | -    |
| options.cpp               | -    |
| options.hpp               | -    |
| own.cpp                   | √    |
| own.hpp                   | √    |
| pair.cpp                  | -    |
| pair.hpp                  | -    |
| peer.cpp                  | -    |
| peer.hpp                  | -    |
| pgm_receiver.cpp          | -    |
| pgm_receiver.hpp          | -    |
| pgm_sender.cpp            | -    |
| pgm_sender.hpp            | -    |
| pgm_socket.cpp            | -    |
| pgm_socket.hpp            | -    |
| pipe.cpp                  | -    |
| pipe.hpp                  | -    |
| plain_client.cpp          | -    |
| plain_client.hpp          | -    |
| plain_common.hpp          | -    |
| plain_server.cpp          | -    |
| plain_server.hpp          | -    |
| platform.hpp.in           | -    |
| poll.cpp                  | -    |
| poller_base.cpp           | -    |
| poller_base.hpp           | -    |
| poller.hpp                | -    |
| poll.hpp                  | -    |
| polling_util.cpp          | -    |
| polling_util.hpp          | -    |
| pollset.cpp               | -    |
| pollset.hpp               | -    |
| precompiled.cpp           | -    |
| precompiled.hpp           | -    |
| proxy.cpp                 | -    |
| proxy.hpp                 | -    |
| pub.cpp                   | -    |
| pub.hpp                   | -    |
| pull.cpp                  | -    |
| pull.hpp                  | -    |
| push.cpp                  | -    |
| push.hpp                  | -    |
| radio.cpp                 | -    |
| radio.hpp                 | -    |
| radix_tree.cpp            | -    |
| radix_tree.hpp            | -    |
| random.cpp                | -    |
| random.hpp                | -    |
| raw_decoder.cpp           | -    |
| raw_decoder.hpp           | -    |
| raw_encoder.cpp           | -    |
| raw_encoder.hpp           | -    |
| raw_engine.cpp            | -    |
| raw_engine.hpp            | -    |
| reaper.cpp                | -    |
| reaper.hpp                | -    |
| rep.cpp                   | -    |
| rep.hpp                   | -    |
| req.cpp                   | -    |
| req.hpp                   | -    |
| router.cpp                | -    |
| router.hpp                | -    |
| scatter.cpp               | -    |
| scatter.hpp               | -    |
| secure_allocator.hpp      | -    |
| select.cpp                | -    |
| select.hpp                | -    |
| server.cpp                | -    |
| server.hpp                | -    |
| session_base.cpp          | -    |
| session_base.hpp          | -    |
| signaler.cpp              | -    |
| signaler.hpp              | -    |
| socket_base.cpp           | -    |
| socket_base.hpp           | √    |
| socket_poller.cpp         | -    |
| socket_poller.hpp         | -    |
| socks_connecter.cpp       | -    |
| socks_connecter.hpp       | -    |
| socks.cpp                 | -    |
| socks.hpp                 | -    |
| stdint.hpp                | -    |
| stream_connecter_base.cpp | -    |
| stream_connecter_base.hpp | -    |
| stream.cpp                | -    |
| stream_engine_base.cpp    | -    |
| stream_engine_base.hpp    | -    |
| stream.hpp                | -    |
| stream_listener_base.cpp  | -    |
| stream_listener_base.hpp  | -    |
| sub.cpp                   | -    |
| sub.hpp                   | -    |
| tcp_address.cpp           | -    |
| tcp_address.hpp           | -    |
| tcp_connecter.cpp         | -    |
| tcp_connecter.hpp         | -    |
| tcp.cpp                   | -    |
| tcp.hpp                   | -    |
| tcp_listener.cpp          | -    |
| tcp_listener.hpp          | -    |
| thread.cpp                | -    |
| thread.hpp                | -    |
| timers.cpp                | -    |
| timers.hpp                | -    |
| tipc_address.cpp          | -    |
| tipc_address.hpp          | -    |
| tipc_connecter.cpp        | -    |
| tipc_connecter.hpp        | -    |
| tipc_listener.cpp         | -    |
| tipc_listener.hpp         | -    |
| trie.cpp                  | -    |
| trie.hpp                  | -    |
| tweetnacl.c               | -    |
| tweetnacl.h               | -    |
| udp_address.cpp           | -    |
| udp_address.hpp           | -    |
| udp_engine.cpp            | -    |
| udp_engine.hpp            | -    |
| v1_decoder.cpp            | -    |
| v1_decoder.hpp            | -    |
| v1_encoder.cpp            | -    |
| v1_encoder.hpp            | -    |
| v2_decoder.cpp            | -    |
| v2_decoder.hpp            | -    |
| v2_encoder.cpp            | -    |
| v2_encoder.hpp            | -    |
| v2_protocol.hpp           | -    |
| v3_1_encoder.cpp          | -    |
| v3_1_encoder.hpp          | -    |
| version.rc.in             | -    |
| vmci_address.cpp          | -    |
| vmci_address.hpp          | -    |
| vmci_connecter.cpp        | -    |
| vmci_connecter.hpp        | -    |
| vmci.cpp                  | -    |
| vmci.hpp                  | -    |
| vmci_listener.cpp         | -    |
| vmci_listener.hpp         | -    |
| windows.hpp               | -    |
| wire.hpp                  | -    |
| ws_address.cpp            | -    |
| ws_address.hpp            | -    |
| ws_connecter.cpp          | -    |
| ws_connecter.hpp          | -    |
| ws_decoder.cpp            | -    |
| ws_decoder.hpp            | -    |
| ws_encoder.cpp            | -    |
| ws_encoder.hpp            | -    |
| ws_engine.cpp             | -    |
| ws_engine.hpp             | -    |
| ws_listener.cpp           | -    |
| ws_listener.hpp           | -    |
| ws_protocol.hpp           | -    |
| wss_address.cpp           | -    |
| wss_address.hpp           | -    |
| wss_engine.cpp            | -    |
| wss_engine.hpp            | -    |
| xpub.cpp                  | -    |
| xpub.hpp                  | -    |
| xsub.cpp                  | -    |
| xsub.hpp                  | -    |
| ypipe_base.hpp            | -    |
| ypipe_conflate.hpp        | -    |
| ypipe.hpp                 | -    |
| yqueue.hpp                | -    |
| zap_client.cpp            | -    |
| zap_client.hpp            | -    |
| zmq.cpp                   | -    |
| zmq_draft.h               | -    |
| zmq_utils.cpp             | -    |
| zmtp_engine.cpp           | -    |
| zmtp_engine.hpp           | -    |