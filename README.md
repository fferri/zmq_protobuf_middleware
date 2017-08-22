A proof of concept middleware using only the ZeroMQ library and Protobuf.

My contribution is entirely in the <a href="protobufhelpers.hpp">protobufhelpers.hpp</a> file, which is a small extension of <a href="zhelpers.hpp">zhelpers.hpp</a>, a small utility library used in the ZMQ C++ examples.

Examples
========

Creating a client/server
========================

We are going to use a REQ/REP socket pair, which means that for each request, the server must answer with a reply, or the protocol will hang.

We want to implement a service for computing the sum of two integer numbers, so we define a `Sum.proto` file with the definitions for the request and the response message.

Sum.proto:
```Protocol Buffer
syntax = "proto2";

package MyMiddleware;

message SumRequest {
  required int32 a = 1;
  required int32 b = 2;
}

message SumResponse {
  required int32 s = 1;
}
```

The server reads a `SumRequest` message, and sends out a `SumResponse` message.

server.cpp:
```C++
    // connect the socket:
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:5555");

    while(true)
    {
        // receive the request message:
        MyMiddleware::SumRequest req;
        s_recv(socket, req);
        std::cout << "received a=" << req.a() << " b=" << req.b() << std::endl;

        // build the response message:
        MyMiddleware::SumResponse resp;
        resp.set_s(req.a() + req.b());

        // send the response:
        s_send(socket, resp);
    }
```

The client sends a `SumRequest` and reads a `SumResponse`.

client.cpp:
```C++
    // connect the socket:
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REQ);
    socket.connect("tcp://localhost:5555");

    // build the request message:
    MyMiddleware::SumRequest req;
    req.set_a(1 + n);
    req.set_b(20 - 4 * n);

    // send request:
    s_send(socket, req);

    // receive the response message:
    MyMiddleware::SumResponse resp;
    s_recv(socket, resp);
    std::cout << "received s=" << resp.s() << std::endl;
```

Creating a publisher/subscriber
=============================

Publisher/subscriber work similarly to previous example, but we set the `ZMQ_PUB` on the publisher endpoint, and `ZMQ_SUB` on the subscriber endpoint. The subscriber will also set up message filtering, with the `subscribe<T>(zmq::socket_t &socket)` call.

For this example we want a publisher broadcasting several messages (Temperature, Status) to all the interested subscribers. A subscriber may be interested only in one type of message (Tempreature).

So we define the two messages: Temperature.proto and Status.proto.

Temperature.proto:
```Protocol Buffer
syntax = "proto2";

package MyMiddleware;

message Temperature {
  required double value = 1;
}
```

Status.proto:
```Protocol Buffer
syntax = "proto2";

package MyMiddleware;

message Status {
  required int32 state = 1;
  required int32 micro_state = 2;
}
```

In order for filtering to work (note: in current ZMQ implementation, filtering works on the subscriber side), messages must have an envelope header (that is, they must be multipart, where the first part is a string with the "address"). The message envelope (containing the Protobuf's message name) will be added automatically by `s_publish(zmq::socket_t &socket, const T &msg)`, and will be extracted by `s_sub_recv(zmq::socket_t &socket, T &msg)`.

subscriber.cpp:
```C++
    // connect the socket:
    zmq::context_t context(1);
    zmq::socket_t subscriber(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:5555");

    // subscribe for MyMiddleware::Temperature messages:
    subscribe<MyMiddleware::Temperature>(subscriber);

    while(true)
    {
        MyMiddleware::Temperature temp;
        if(s_sub_recv(subscriber, temp))
            std::cout << "temperature: value=" << req.value() << std::endl;
    }
```

publisher.cpp:
```C++
    // connect the socket:
    zmq::context_t context(1);
    zmq::socket_t publisher(context, ZMQ_PUB);
    publisher.bind("tcp://*:5555");

    // build the message to publish:
    MyMiddleware::Temperature temp;
    temp.set_value(100);

    // publish the message:
    s_publish(publisher, temp);
```

If we want to publish multiple types on the same socket, we need a more complex code (see multi-subscriber.cpp). My recommendation is to have one socket for each type (like ROS does).

