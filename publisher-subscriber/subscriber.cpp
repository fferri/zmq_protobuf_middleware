#include <zmq.hpp>
#include <string>
#include <iostream>
#include "Temperature.pb.h"
#include "zhelpers.hpp"
#include "protobufhelpers.hpp"

int main()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5555");
    subscribe<MyMiddleware::Temperature>(subscriber);

    while(true)
    {
        MyMiddleware::Temperature temp;
        if(s_sub_recv(subscriber, temp))
            std::cout << "message: temperature: value=" << temp.value() << std::endl;
    }
    return 0;
}

