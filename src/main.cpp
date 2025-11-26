#include "websocket/WebSocketServer.h"
#include <boost/asio.hpp>
#include <iostream>

int main()
{
    try {
        boost::asio::io_context ioc;

        WebSocketServer server(ioc, 8080);

        std::cout << "Server started at ws://localhost:8080\n";

        server.run();

        ioc.run();
    }
    catch (std::exception& e) {
        std::cerr << "Fatal: " << e.what() << "\n";
    }
}
