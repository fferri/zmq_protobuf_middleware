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

    // Here we publish messages of different type on the same publisher.
    // The messages can be distinguished by looking at the envelope address
    // (see the implementation of s_publish()).
    while(true)
    {
        // publish a Temperature message:
        MyMiddleware::Temperature temp;
        temp.set_value(28.57);
        s_publish(publisher, temp);

        // publish a Status message:
        MyMiddleware::Status status;
        status.set_state(1);
        status.set_micro_state(1);
        s_publish(publisher, status);

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
    return 0;
}

