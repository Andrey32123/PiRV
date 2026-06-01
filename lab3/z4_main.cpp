//g++ -std=c++17 -o task4.exe z4_main.cpp -lws2_32 -lmswsock -lboost_system-mgw13-mt-x64-1_87 -I C:\local\boost_1_87_0 -L C:\local\boost_1_87_0\stage\lib
#include <iostream>
#include <vector>
#include <thread>
#include <memory>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

unsigned long long factorial(int n) {
    unsigned long long r = 1;
    for (int i = 2; i <= n; ++i) r *= i;
    return r;
}

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket s,
            boost::asio::strand<boost::asio::io_context::executor_type>& ls,
            std::vector<std::string>& log,
            boost::asio::thread_pool& pool)
        : socket_(std::move(s)), log_strand_(ls), global_log_(log), calc_pool_(pool), timer_(socket_.get_executor()) {}

    void start() {
        timer_.expires_after(std::chrono::seconds(30));
        timer_.async_wait([this](boost::system::error_code ec) { if (!ec) socket_.close(); });
        do_read();
    }

private:
    tcp::socket socket_;
    boost::asio::strand<boost::asio::io_context::executor_type>& log_strand_;
    std::vector<std::string>& global_log_;
    boost::asio::thread_pool& calc_pool_;
    boost::asio::steady_timer timer_;
    char data_[1024] = {0};

    void do_read() {
        auto self = shared_from_this();
        socket_.async_read_some(boost::asio::buffer(data_, 1024),
            [this, self](boost::system::error_code ec, size_t len) {
                if (!ec) {
                    timer_.cancel();
                    std::string msg(data_, len);
                    boost::asio::post(calc_pool_, [this, self, msg]() {
                        int n = std::stoi(msg);
                        std::string res = std::to_string(factorial(n)) + "\n";
                        boost::asio::post(log_strand_, [this, self, msg, res]() {
                            global_log_.push_back("Factorial(" + msg + ")=" + res);
                        });
                        boost::asio::async_write(socket_, boost::asio::buffer(res),
                            [this, self](boost::system::error_code, size_t) { do_read(); });
                    });
                }
            });
    }
};

class Server {
public:
    Server(boost::asio::io_context& io, short port, int threads)
        : acceptor_(io, tcp::endpoint(tcp::v4(), port)), log_strand_(io.get_executor()), calc_pool_(threads) {
        do_accept();
    }
private:
    tcp::acceptor acceptor_;
    boost::asio::strand<boost::asio::io_context::executor_type> log_strand_;
    std::vector<std::string> global_log_;
    boost::asio::thread_pool calc_pool_;
    void do_accept() {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket s) {
            if (!ec) std::make_shared<Session>(std::move(s), log_strand_, global_log_, calc_pool_)->start();
            do_accept();
        });
    }
};

int main() {
    std::cout << "Factorial calculator. Enter numbers (0 to exit):\n";
    
    int threads = std::thread::hardware_concurrency();
    if (threads == 0) threads = 2;
    boost::asio::io_context io;
    Server srv(io, 12345, threads);
    std::vector<std::thread> workers;
    for (int i = 0; i < threads; ++i) workers.emplace_back([&io]() { io.run(); });
    
    boost::asio::io_context io2;
    tcp::socket sock(io2);
    sock.connect(tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), 12345));
    
    std::string input;
    while (std::cin >> input && input != "0") {
        sock.write_some(boost::asio::buffer(input + "\n"));
        char resp[256];
        size_t len = sock.read_some(boost::asio::buffer(resp));
        std::cout << std::string(resp, len);
    }
    
    for (auto& t : workers) t.detach();
    return 0;
}