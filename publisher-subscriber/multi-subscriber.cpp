#include <zmq.hpp>
#include <string>
#include <iostream>
#include "Temperature.pb.h"
#include "Status.pb.h"
#include "zhelpers.hpp"
#include "protobufhelpers.hpp"

int main()
{
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5555");
    subscribe<MyMiddleware::Temperature>(subscriber);
    subscribe<MyMiddleware::Status>(subscriber);

    while(true)
    {
        MyMiddleware::Temperature temp;
        MyMiddleware::Status status;
        std::string env = s_recv(subscriber);
        std::string msg_str = s_recv(subscriber);
        // here we look at the 0MQ envelope address field (env) to know which
        // type of message is the payload (msg_str) because a serialized ProtoBuf
        // message is not self-descriptive
        if(env == "MyMiddleware.Temperature" && temp.ParseFromString(msg_str))
            std::cout << "message: temperature: value=" << temp.value() << std::endl;
        else if(env == "MyMiddleware.Status" && status.ParseFromString(msg_str))
            std::cout << "message: status: state=" << status.state() << " micro_state=" << status.micro_state() << std::endl;
        else
            std::cout << "deserialization failed or unknown message type" << std::endl;
    }
    return 0;
}

