// boost_server.cpp
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sstream>

using boost::asio::ip::tcp;

std::string search_max(const std::string& input) {
    std::istringstream iss(input);
    int max = 0, num;
    iss >> max;
    while (iss >> num) {
        if (num > max) {
            max = num;
        }
    }

    std::string result = "Maximum: " + std::to_string(max);

    return result;
}

void session(tcp::socket socket) {
    try {

        boost::asio::streambuf buffer;

        boost::system::error_code ec;
        while (true) {
            size_t length = boost::asio::read_until(socket, buffer, "\n", ec);

            std::istream input_stream(&buffer);
            std::string client_message;
            std::getline(input_stream, client_message);

            if (client_message == "quit") {
                std::cout << "Closing connection..." << std::endl;
                break;
            }

            std::string res = search_max(client_message);

            std::cout << res << std::endl;
        }

    }
    catch (std::exception& e) {
        std::cerr << "Session error: " << e.what() << std::endl;
    }
}

void server(boost::asio::io_context& io_context, unsigned short port) {
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
    std::cout << "Server is listening on port " << port << "..." << std::endl;

    while (true) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "Client connected!" << std::endl;
        session(std::move(socket));
    }
}

int main() {
    try {
        boost::asio::io_context io_context;
        server(io_context, 12345);
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
