// boost_server.cpp
#include <boost/asio.hpp>
#include <iostream>
#include <string>

using boost::asio::ip::tcp;

std::string caps_lock(const std::string& input) {
    // Переводим строку в верхний регистр
    std::string upper_str = input;
    std::transform(upper_str.begin(), upper_str.end(), upper_str.begin(),
        [](unsigned char c) { return std::toupper(c); });

    // Формируем результат: <длина>: <строка в верхнем регистре>
    std::string result = std::to_string(input.length()) + ": " + upper_str;

    return result;
}

// Функция для общения с одним подключенным клиентом
void session(tcp::socket socket) {
    try {
        
        boost::asio::streambuf buffer;

        boost::system::error_code ec;
        while(true){
            size_t length = boost::asio::read_until(socket, buffer, "\n", ec);

            // Извлекаем строку из буфера
            std::istream input_stream(&buffer);
            std::string client_message;
            std::getline(input_stream, client_message);

            if (client_message == "quit") {
                std::cout << "Closing connection..." << std::endl;
                break;
            }

            std::string res = caps_lock(client_message);

            std::cout << res << std::endl;
        }
        
    }
    catch (std::exception& e) {
        std::cerr << "Session error: " << e.what() << std::endl;
    }
}

void server(boost::asio::io_context& io_context, unsigned short port) {
    // Создаем acceptor, который будет слушать порт
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), port));
    std::cout << "Server is listening on port " << port << "..." << std::endl;

    while(true){
        tcp::socket socket(io_context);
        // Ждем подключения клиента
        acceptor.accept(socket);
        std::cout << "Client connected!" << std::endl;
        // Обрабатываем клиента
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