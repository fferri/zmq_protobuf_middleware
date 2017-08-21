#include <zmq.hpp>
#include <string>
#include <iostream>
#include "messages.pb.h"
#include "zhelpers.hpp"

int main()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");

    for(int n = 0; n < 3; n++) {
        // build SumRequest:
        MyMiddleware::SumRequest req;
        req.set_a(1 + n);
        req.set_b(20 - 4 * n);

        // send SumRequest:
        std::cout << "sending a=" << req.a() << " b=" << req.b() << std::endl;
        std::string req_str;
        if(req.SerializeToString(&req_str))
            s_send(socket, req_str);
        else
            std::cout << "failed serialization" << std::endl;

        // receive SumResponse:
        MyMiddleware::SumResponse resp;
        std::string resp_str = s_recv(socket);
        if(resp.ParseFromString(resp_str))
            std::cout << "received s=" << resp.s() << std::endl;
        else
            std::cout << "failed deserialization" << std::endl;
    }
    return 0;
}

