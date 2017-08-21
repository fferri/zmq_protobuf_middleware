MyMiddleware
============

A proof of concept middleware using only the ZeroMQ library and Protobuf.

Examples
========

Creating a client/server
========================

For each request the server must answer with a reply, or the protocol will hang.

server.cpp:
```
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

client.cpp:
```
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

Creating a publish/subscriber
=============================

The message envelope with the Protobuf's message name will be added automatically by `s_publish(zmq::socket_t &socket, const T &msg)`, and will be extracted by `s_sub_recv(zmq::socket_t &socket, T &msg)`.

subscriber.cpp:
```
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
```
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

