#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <boost/format.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include "messages.pb.h"
#include "zhelpers.hpp"

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
        std::string env1 = "SumRequest";
        std::string req_str;
        req_str = "REQ";
        if(req.SerializeToString(&req_str))
        {
            s_sendmore(publisher, env1);
            s_send(publisher, req_str);
            std::cout << "send [" << env1 << "] a=" << req.a() << " b=" << req.b() << std::endl;
        }
        else
            std::cout << "serialization failed" << std::endl;

        // build SumResponse:
        MyMiddleware::SumResponse resp;
        resp.set_s(1000);

        // send SumResponse:
        std::string env2 = "SumResponse";
        std::string resp_str;
        if(resp.SerializeToString(&resp_str))
        {
            s_sendmore(publisher, env2);
            s_send(publisher, resp_str);
            std::cout << "send [" << env2 << "] s=" << resp.s() << std::endl;
        }
        else
            std::cout << "serialization failed" << std::endl;

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    }
    return 0;
}

