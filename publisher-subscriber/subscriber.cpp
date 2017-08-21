#include <zmq.hpp>
#include <string>
#include <iostream>
#include "messages.pb.h"
#include "zhelpers.hpp"

int main()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5555");
    std::string topic = "SumRequest";
    subscriber.setsockopt(ZMQ_SUBSCRIBE, topic.data(), topic.size());

    while(true)
    {
        std::string env = s_recv(subscriber); // message envelope
        std::string req_str = s_recv(subscriber);

        MyMiddleware::SumRequest req;
        if(req.ParseFromString(req_str))
            std::cout << "[" << env << "] message: a=" << req.a() << " b=" << req.b() << std::endl;
        else
            std::cout << "deserialization failed" << std::endl;
    }
    return 0;
}

