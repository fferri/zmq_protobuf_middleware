std::string s_recv(zmq::socket_t &socket)
{
    zmq::message_t zmqmsg;
    socket.recv(&zmqmsg);
    std::string msgstr(static_cast<char*>(zmqmsg.data()), zmqmsg.size());
    return msgstr;
}

void s_send(zmq::socket_t &socket, const std::string &s)
{
    zmq::message_t zmqmsg(s.c_str(), s.size());
    socket.send(zmqmsg);
}

void s_sendmore(zmq::socket_t &socket, const std::string &s)
{
    zmq::message_t zmqmsg(s.c_str(), s.size());
    socket.send(zmqmsg, ZMQ_SNDMORE);
}

template<typename T>
void x_recv(zmq::socket_t &socket, T &msg)
{
    msg.ParseFromString(s_recv(socket));
}

template<typename T>
void x_send(zmq::socket_t &socket, const T &msg)
{
    std::string msgstr;
    msg.SerializeToString(&msgstr);
    s_send(socket, msgstr);
}

