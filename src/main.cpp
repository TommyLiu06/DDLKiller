#include "websocket/WebSocketServer.h"
#include <boost/asio.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc == 3) {
        unsigned short port = static_cast<unsigned short>(std::stoi(argv[1]));
        const char* dbPath = argv[2];

        try {
            boost::asio::io_context ioc;
            WebSocketServer server(ioc, port, dbPath);

            std::cout << "Server started at ws://localhost:" << port << "\n";

            server.run();
            ioc.run();

        } catch (std::exception& e) {
            std::cerr << "Fatal: " << e.what() << "\n";
        }

        return 0;

    } else if (argc == 2 && std::string(argv[1]) == "--help") {
        std::cout << "Usage: ddl_sync_server [prot] [database_path]\n"
                  << "Options:\n"
                  << "  --help        Show this help message\n";

        return 0;

    } else {
        std::cerr << "Invalid arguments. Use --help for usage information.\n";
        return 1;
    }
}
