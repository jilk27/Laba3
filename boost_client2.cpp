// boost_client.cpp
#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

void client(boost::asio::io_context& io_context, const std::string& host, const std::string& port) {
    tcp::resolver resolver(io_context);
    // Преобразуем имя хоста и порт в IP-адреса
    auto endpoints = resolver.resolve(host, port);

    tcp::socket socket(io_context);
    // Подключаемся к серверу
    boost::asio::connect(socket, endpoints);
    std::cout << "Connected to server!" << std::endl;

    boost::system::error_code ec;

    while (true) {
        // 2. Отправляем данные серверу
        std::string message;
        std::getline(std::cin, message);

        if (message == "quit") {
            std::cout << "Closing connection..." << std::endl;
            boost::asio::write(socket, boost::asio::buffer(message), ec);
            break;
        }

        message += '\n';

        boost::asio::write(socket, boost::asio::buffer(message), ec);
        if (ec) {
            std::cerr << "Error sending data: " << ec.message() << std::endl;
        }
        else {
            std::cout << "Client sent: " << message << std::endl;
        }
    }
}

int main() {
    try {
        boost::asio::io_context io_context;
        client(io_context, "127.0.0.1", "12345");
        std::cout << "\nPress Enter to exit...";
        std::cin.get();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        std::cin.get();
    }
    return 0;
}