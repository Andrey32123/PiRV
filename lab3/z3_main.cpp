//g++ -std=c++17 -o task3.exe z3_main.cpp -lws2_32 -lmswsock -lboost_system-mgw13-mt-x64-1_87 -lboost_thread-mgw13-mt-x64-1_87 -I C:\local\boost_1_87_0 -L C:\local\boost_1_87_0\stage\lib

#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <thread>
#include <chrono>

using boost::asio::ip::tcp;

void server3() {
    boost::asio::io_context io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 12347));
    std::cout << "Server started. Waiting...\n";
    
    tcp::socket socket(io);
    acceptor.accept(socket);
    
    char data[1024];
    size_t len = socket.read_some(boost::asio::buffer(data));
    std::string request(data, len);
    
    if (request.find("timer") == 0) {
        int seconds = std::stoi(request.substr(6));
        std::cout << "Timer started for " << seconds << " sec\n";
        
        std::string ack = "Timer started for " + std::to_string(seconds) + " sec\n";
        boost::asio::write(socket, boost::asio::buffer(ack));
        
        std::this_thread::sleep_for(std::chrono::seconds(seconds));
        
        std::string response = "Time's up after " + std::to_string(seconds) + " sec!\n";
        boost::asio::write(socket, boost::asio::buffer(response));
    }
}

void client3() {
    boost::asio::io_context io;
    tcp::resolver resolver(io);
    auto endpoints = resolver.resolve("127.0.0.1", "12347");
    
    tcp::socket socket(io);
    boost::asio::connect(socket, endpoints);
    
    std::cout << "Connected. Enter 'timer N': ";
    std::string msg;
    std::getline(std::cin, msg);
    msg += "\n";
    
    boost::asio::write(socket, boost::asio::buffer(msg));
    
    char response[1024];
    size_t len = socket.read_some(boost::asio::buffer(response));
    std::cout << std::string(response, len);
    
    len = socket.read_some(boost::asio::buffer(response));
    std::cout << std::string(response, len);
}

int main() {
    std::cout << "\nT 3\n";
    std::thread t(server3);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    client3();
    t.detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}