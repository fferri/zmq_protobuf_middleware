#include <zmq.hpp>
#include <string>
#include <iostream>
#include "messages.pb.h"
#include "zhelpers.hpp"
#include "protobufhelpers.hpp"

int main()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5555");
    subscribe<MyMiddleware::SumRequest>(subscriber);

    while(true)
    {
        MyMiddleware::SumRequest req;
        if(s_sub_recv(subscriber, req))
            std::cout << "message: a=" << req.a() << " b=" << req.b() << std::endl;
        else
            std::cout << "deserialization failed" << std::endl;
    }
    return 0;
}

