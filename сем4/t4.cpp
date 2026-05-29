// task4
#include <fstream>
#include <mutex>
#include <thread>
#include <iostream>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>

template<typename T>
class Logger {
private:
    std::ofstream file;
    std::mutex file_mtx;
    std::mutex cout_mtx;

    inline std::string to_string_impl(const T& msg) {
        std::stringstream ss;
        ss << msg;
        return ss.str();
    }

public:
    Logger(const std::string& filename) {
        file.open(filename, std::ios::out | std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Failed to open log file: " << filename << std::endl;
        }
    }

    ~Logger() {
        if (file.is_open()) {
            file.close();
        }
    }

    void log(const T& message) {
        std::string str = to_string_impl(message);
        auto id = std::this_thread::get_id();
        
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        
        std::stringstream output;
        output << "[" << std::ctime(&time_t_now) << "] Thread " << id << ": " << str;
        std::string full_message = output.str();
        
        full_message.pop_back();

        {
            std::lock_guard<std::mutex> lock(file_mtx);
            if (file.is_open()) {
                file << full_message << std::endl;
                file.flush();
            }
        }

        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << full_message << std::endl;
        }

        std::this_thread::yield();
    }
};

int main() {
    Logger<std::string> logger("application_log.txt");
    
    std::cout << "Starting multi-threaded logging test...\n" << std::endl;

    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i) {
        threads.push_back(std::thread([&logger, i]() {
            for (int j = 0; j < 5; ++j) {
                std::string msg = "[Worker-" + std::to_string(i) + 
                                 "] Processing item #" + std::to_string(j) + 
                                 " (iteration " + std::to_string(j * 2) + ")";
                logger.log(msg);
                std::this_thread::sleep_for(std::chrono::milliseconds(15 * (i + 1)));
            }
            
            logger.log("[Worker-" + std::to_string(i) + "] Completed all tasks");
        }));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "\nAll threads completed. Check 'application_log.txt' file." << std::endl;
    
    logger.log("APPLICATION SHUTDOWN");

    return 0;
}