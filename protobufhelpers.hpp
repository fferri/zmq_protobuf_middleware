template<typename T>
static bool
s_recv(zmq::socket_t &socket, T &msg)
{
    std::string msg_str = s_recv(socket);
    return msg.ParseFromString(msg_str);
}

template<typename T>
static bool
s_send(zmq::socket_t &socket, const T &msg)
{
    std::string msg_str;
    if(msg.SerializeToString(&msg_str))
    {
        s_send(socket, msg_str);
        return true;
    }
    else return false;
}

template<typename T>
static bool
s_publish(zmq::socket_t &publisher, const T &msg)
{
    std::string msg_str;
    if(msg.SerializeToString(&msg_str))
    {
        s_sendmore(publisher, msg.GetTypeName());
        s_send(publisher, msg_str);
        return true;
    }
    else return false;
}

template<typename T>
static bool
s_sub_recv(zmq::socket_t &subscriber, std::string &topic, T &msg)
{
    topic = s_recv(subscriber);
    std::string msg_str = s_recv(subscriber);
    return msg.ParseFromString(msg_str);
}

template<typename T>
static bool
s_sub_recv(zmq::socket_t &publisher, T &msg)
{
    std::string topic;
    return s_sub_recv(publisher, topic, msg);
}

template<typename T>
static void
subscribe(zmq::socket_t &subscriber)
{
    T msg;
    std::string topic = msg.GetTypeName();
    subscriber.setsockopt(ZMQ_SUBSCRIBE, topic.data(), topic.size());
}

