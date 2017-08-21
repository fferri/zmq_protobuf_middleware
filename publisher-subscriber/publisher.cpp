#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/format.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include "messages.pb.h"
#include "zhelpers.hpp"
#include "protobufhelpers.hpp"

int main()
{
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5555");

    for(int n = 0; n < 3; n++) {

        // build SumRequest:
        MyMiddleware::SumRequest req;
        req.set_a(10);
        req.set_b(100 + n);

        // send SumRequest:
        if(s_publish(publisher, req))
            std::cout << "send a=" << req.a() << " b=" << req.b() << std::endl;
        else
            std::cout << "serialization failed" << std::endl;

        // build SumResponse:
        MyMiddleware::SumResponse resp;
        resp.set_s(1000);

        // send SumResponse:
        if(s_publish(publisher, resp))
            std::cout << "send s=" << resp.s() << std::endl;
        else
            std::cout << "serialization failed" << std::endl;

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
    return 0;
}

