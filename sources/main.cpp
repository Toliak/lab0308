#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "LogSetup.h"
#include "core/Server.h"

Server::Endpoint getEndpoint(char *host, char *port)
{
    Server::IoService ioService;

    boost::asio::ip::tcp::resolver resolver(ioService);
    boost::asio::ip::tcp::resolver::query query(host, port);

    boost::asio::ip::tcp::resolver::iterator endpoint = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end;

    if (endpoint == end) {
        throw std::runtime_error{"Host and port cannot be resolved"};
    }

    return *endpoint;
}

int main(int argc, char *argv[])
{
    LogSetup::init();

    BOOST_LOG_TRIVIAL(debug) << "Log setup complete";

    if (argc != 3) {
        BOOST_LOG_TRIVIAL(error) << "Expected: client <host> <port>";
        return 1;
    }

    for (;;) {
        BOOST_LOG_TRIVIAL(info) << "Client has been started";

        try {
            static Server::Endpoint endpoint = getEndpoint(argv[1], argv[2]);

            Server server{endpoint};
            server.act();

        } catch (std::runtime_error &error) {
            BOOST_LOG_TRIVIAL(error) << error.what();
        }
    }
}