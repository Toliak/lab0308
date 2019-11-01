#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "LogSetup.h"

int main(int argc, char *argv[])
{
    LogSetup::init();

    BOOST_LOG_TRIVIAL(info) << "Я программа";

    if (argc != 2)
    {
        BOOST_LOG_TRIVIAL(error) << "Expected: client <host>";
        return 1;
    }

    boost::asio::io_service io_service;

    boost::asio::ip::tcp::resolver resolver(io_service);
    boost::asio::ip::tcp::resolver::query query(argv[1], "daytime");
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    boost::asio::ip::tcp::resolver::iterator end;

    boost::asio::ip::tcp::socket socket(io_service);

    while (endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++);
    }

    for (;;)
    {
        boost::array<char, 128> buf;
        boost::system::error_code error;

        size_t len = socket.read_some(boost::asio::buffer(buf), error);

        if (error == boost::asio::error::eof)
            break; // Connection closed cleanly by peer.
        else if (error)
            throw boost::system::system_error(error); // Some other error.

        std::cout.write(buf.data(), len);
    }
}