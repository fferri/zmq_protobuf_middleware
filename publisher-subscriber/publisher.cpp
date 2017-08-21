#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/format.hpp>
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
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5555");

    for(int n = 0; n < 3; n++) {

        // build SumRequest:
        MyMiddleware::Temperature temp;
        temp.set_value(28.57);

        // send SumRequest:
        if(s_publish(publisher, temp))
            std::cout << "send temp=" << temp.value() << std::endl;
        else
            std::cout << "serialization failed" << std::endl;

        // build SumResponse:
        MyMiddleware::Status status;
        status.set_state(1);
        status.set_micro_state(1);

        // send SumResponse:
        if(s_publish(publisher, status))
            std::cout << "send state=" << status.state() << " micro_state=" << status.micro_state() << std::endl;
        else
            std::cout << "serialization failed" << std::endl;

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
    return 0;
}

