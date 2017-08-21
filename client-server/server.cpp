#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include "messages.pb.h"
#include "zhelpers.hpp"

int main()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    while(true)
    {
        // receive SumRequest:
        MyMiddleware::SumRequest req;
        std::string req_str = s_recv(socket);
        if(req.ParseFromString(req_str))
            std::cout << "received a=" << req.a() << " b=" << req.b() << std::endl;
        else
            std::cout << "failed deserialization" << std::endl;

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

        // build SumResponse:
        MyMiddleware::SumResponse resp;
        resp.set_s(req.a() + req.b());

        // send SumResponse:
        std::cout << "sending s=" << resp.s() << std::endl;
        std::string resp_str;
        if(resp.SerializeToString(&resp_str))
            s_send(socket, resp_str);
        else
            std::cout << "failed serialization" << std::endl;
    }
    return 0;
}

