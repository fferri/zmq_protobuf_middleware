#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/format.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include "messages.pb.h"
#include "utils.h"

int main()
{
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5555");

    for(int n = 0; n < 3; n++) {
        {
            // send the message envelope:
            s_sendmore(publisher, "SumRequest");

            // build SumRequest:
            MyMiddleware::SumRequest req;
            req.set_a(10);
            req.set_b(100 + n);

            // send SumRequest:
            std::cout << "sending a=" << req.a() << " b=" << req.b() << std::endl;
            x_send(publisher, req);
        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
    return 0;
}

