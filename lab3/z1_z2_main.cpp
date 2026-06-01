// ЛР3_пирв.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//g++ -std=c++17 -o program.exe z1_z2_main.cpp -lws2_32 -lboost_system-mt -lboost_thread-mt -I C:\local\boost_1_87_0 -L C:\local\boost_1_87_0\stage\lib
// //g++ .cpp -o program.exe -L C:\local\boost_1_87_0\stage\lib -lboost_thread -I C:\local\boost_1_87_0
// ЛР3_пирв.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
// Компиляция: g++ -std=c++17 -o program.exe .cpp -lws2_32 -lboost_system-mt -lboost_thread-mt
// -I C:\local\boost_1_87_0 -L C:\local\boost_1_87_0\stage\lib


// g++ -std=c++17 -o program.exe z1_z2_main.cpp -lws2_32 -lmswsock -lwsock32 -lboost_system-mgw13-mt-x64-1_87 -lboost_thread-mgw13-mt-x64-1_87 -I C:\local\boost_1_87_0 -L C:\local\boost_1_87_0\stage\lib

// ЛР3_пирв.cpp
// ЛР3_пирв.cpp
#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <chrono>

using boost::asio::ip::tcp;

unsigned long long factorial(int n) {
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

// ==================== ЗАДАНИЕ 1 ====================
void server1() {
    boost::asio::io_context io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 12345));
    std::cout << "Server started. Waiting...\n";
    
    tcp::socket socket(io);
    acceptor.accept(socket);
    
    char data[1024];
    size_t len = socket.read_some(boost::asio::buffer(data));
    std::string msg(data, len);
    
    std::string response = "Message received: " + msg + "\n";
    boost::asio::write(socket, boost::asio::buffer(response));
}

void client1() {
    boost::asio::io_context io;
    tcp::resolver resolver(io);
    auto endpoints = resolver.resolve("127.0.0.1", "12345");
    
    tcp::socket socket(io);
    boost::asio::connect(socket, endpoints);
    
    std::cout << "Enter message: ";
    std::string msg;
    std::getline(std::cin, msg);
    msg += "\n";
    
    boost::asio::write(socket, boost::asio::buffer(msg));
    
    char response[1024];
    size_t len = socket.read_some(boost::asio::buffer(response));
    std::cout << "Response: " << std::string(response, len);
}

// ==================== ЗАДАНИЕ 2 ====================
class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {}
    
    void start() {
        do_read();
    }
    
private:
    tcp::socket socket_;
    char data_[1024] = {0};
    
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, 1024),
            [this, self](boost::system::error_code ec, size_t len) {
                if (!ec) {
                    std::string msg(data_, len);
                    
                    std::string ack = "Accepted, calculating...\n";
                    boost::asio::write(socket_, boost::asio::buffer(ack));
                    
                    boost::asio::post(socket_.get_executor(), [this, self, msg]() {
                        int num = std::stoi(msg);
                        std::string result = std::to_string(factorial(num)) + "\n";
                        
                        boost::asio::async_write(socket_, boost::asio::buffer(result),
                            [this, self](boost::system::error_code, size_t) {});
                    });
                    
                    do_read();
                }
            });
    }
};

class Server {
public:
    Server(boost::asio::io_context& io, short port)
        : acceptor_(io, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }
    
private:
    tcp::acceptor acceptor_;
    
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->start();
                }
                do_accept();
            });
    }
};

void server2() {
    boost::asio::io_context io;
    Server srv(io, 12346);
    io.run();
}

void client2() {
    boost::asio::io_context io;
    tcp::resolver resolver(io);
    auto endpoints = resolver.resolve("127.0.0.1", "12346");
    
    tcp::socket socket(io);
    boost::asio::connect(socket, endpoints);
    
    while (true) {
        std::cout << "\nEnter number (0 to exit): ";
        std::string input;
        std::cin >> input;
        
        if (input == "0") break;
        
        input += "\n";
        boost::asio::write(socket, boost::asio::buffer(input));
        
        char ack[256];
        socket.read_some(boost::asio::buffer(ack));
        std::cout << ack;
        
        char result[256];
        size_t len = socket.read_some(boost::asio::buffer(result));
        std::cout << "Factorial: " << std::string(result, len);
    }
}

// ==================== MAIN ====================
int main() {
    std::cout << "\n=== Task 1 ===\n";
    std::thread t1(server1);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    client1();
    t1.detach();
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    std::cout << "\n=== Task 2 ===\n";
    std::thread t2(server2);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    client2();
    t2.detach();
    
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}