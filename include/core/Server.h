#pragma once

#include <boost/asio.hpp>

class Server
{
public:
    using Endpoint = boost::asio::ip::tcp::endpoint;
    using IoService = boost::asio::io_service;
    using Socket = boost::asio::ip::tcp::socket;

    explicit Server(const Endpoint &endpoint);
    std::string sendAndReceive(std::string);

    void act();

private:
    IoService ioService_{};
    Socket socket_{ioService_};
};