#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
#include <zmq.hpp>
#include <string>
#include <iostream>
#include "messages.pb.h"
#include "utils.h"

int main()
{
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    while(true)
    {
        // receive SumRequest:
        MyMiddleware::SumRequest req;
        x_recv(socket, req);
        std::cout << "received a=" << req.a() << " b=" << req.b() << std::endl;

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));

        // build SumResponse:
        MyMiddleware::SumResponse resp;
        resp.set_s(req.a() + req.b());

        // send SumResponse:
        std::cout << "sending s=" << resp.s() << std::endl;
        x_send(socket, resp);
    }
    return 0;
}

