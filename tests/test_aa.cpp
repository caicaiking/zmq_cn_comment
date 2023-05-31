//
// Created by acai on 23-1-30.
//

#include "testutil.hpp"
#include "testutil_unity.hpp"
#include <zmq_addon.hpp>
#include "../src/log.hpp"
#include <string.h>

int main(int argc, char **argv) {
    LOG_I("start log");
    auto *ctx = zmq_ctx_new();
    auto socket = zmq_socket(ctx, ZMQ_DEALER);

    zmq_connect(socket, "tcp://127.0.0.1:50059");

    while (true) {

        zmq::multipart_t messages;

        messages.addstr("");
        LOG_I("send message to router");
        messages.send(zmq::socket_ref(zmq::from_handle, socket));
        zmq_sleep(10);
        messages.recv(zmq::socket_ref(zmq::from_handle, socket));

    }
    zmq_sleep(10000);

    zmq_close(socket);
    zmq_ctx_term(ctx);
}