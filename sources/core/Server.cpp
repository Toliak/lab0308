#include <array>
#include <chrono>
#include <boost/log/trivial.hpp>
#include <random>

#include "Utils.h"
#include "Server.h"
#include "ServerException.h"


Server::Server(const Server::Endpoint &endpoint)
{
    socket_.connect(endpoint);
}

std::string Server::sendAndReceive(std::string toSend)
{
    socket_.write_some(boost::asio::buffer(toSend));
    BOOST_LOG_TRIVIAL(debug) << "Sent: " << toSend;

    std::string received;
    boost::asio::read_until(socket_, boost::asio::dynamic_buffer(received), "\n");
    BOOST_LOG_TRIVIAL(debug) << "Received: " << received;

    return received;
}

const std::array<void (*)(Server &), 3> functions = {
    [](Server &server)                              // Login
    {
        std::string login = generateRandomLogin();
        std::string toSend = "login " + login + "\n";
        std::string received = server.sendAndReceive(toSend);

        if (received != "login ok\n") {
            throw ServerException{"Wrong output"};
        }

        BOOST_LOG_TRIVIAL(info) << "Login as '" << login << "' OK";
    },
    [](Server &server)                              // Client list
    {
        std::string received = server.sendAndReceive("clients\n");

        BOOST_LOG_TRIVIAL(info) << "Client list: " << received.substr(0, received.size() - 1);
    },
    [](Server &server)
    {
        static std::mt19937 generator{std::random_device{}()};
        static std::uniform_int_distribution<int> getSeconds{1, 7};

        for (;;) {
            int seconds = getSeconds(generator);
            BOOST_LOG_TRIVIAL(info) << "Sleep: " << seconds << "s";

            std::this_thread::sleep_for(std::chrono::seconds{seconds});
            std::string received = server.sendAndReceive("ping\n");

            if (received != "ping ok\n") {
                BOOST_LOG_TRIVIAL(info) << "Client list has changed";

                functions[1](server);
                continue;
            }

            BOOST_LOG_TRIVIAL(info) << "Ping OK";
        }
    },
};

void Server::act()
{
    for (auto &function : functions) {
        function(*this);
    }
}
