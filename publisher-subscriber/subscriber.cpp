#include <zmq.hpp>
#include <string>
#include <iostream>
#include "messages.pb.h"
#include "utils.h"

int main()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5555");
    subscriber.setsockopt(ZMQ_SUBSCRIBE, "SumRequest", 1);

    while(true)
    {
        // receive message envelope:
        std::string env = s_recv(subscriber);
        //std::cout << "Received envelope '" << env << "'" << std::endl;

        // receive SumRequest:
        MyMiddleware::SumRequest req;
        x_recv(subscriber, req);
        std::cout << "received a=" << req.a() << " b=" << req.b() << std::endl;
    }
    return 0;
}

