#include <zmq.hpp>
#include <string>
#include <iostream>
#include "messages.pb.h"
#include "utils.h"

int main()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);

    std::cout << "Connecting to hello world server..." << std::endl;
    socket.connect("tcp://localhost:5555");

    for(int n = 0; n < 3; n++) {
        // build SumRequest:
        MyMiddleware::SumRequest req;
        req.set_a(1 + n);
        req.set_b(20 - 4 * n);

        // send SumRequest:
        std::cout << "sending a=" << req.a() << " b=" << req.b() << std::endl;
        x_send(socket, req);

        // receive SumResponse:
        MyMiddleware::SumResponse resp;
        x_recv(socket, resp);
        std::cout << "received s=" << resp.s() << std::endl;
    }
    return 0;
}

