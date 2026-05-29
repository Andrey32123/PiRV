// task5
#include <map>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

template<typename Key, typename Value>
class Cache {
private:
    std::map<Key, Value> data;
    std::mutex mtx;
    std::condition_variable cv;

    inline bool key_exists(const Key& k) {
        return data.find(k) != data.end();
    }

public:
    void set(const Key& key, const Value& value) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            data[key] = value;
            std::cout << "[SET] Thread " << std::this_thread::get_id()
                      << " : " << key << " = " << value << std::endl;
            cv.notify_all();
        }
        std::this_thread::yield();
    }

    Value get(const Key& key) {
        std::unique_lock<std::mutex> lock(mtx);
        auto id = std::this_thread::get_id();

        std::cout << "[GET] Thread " << id << " waiting for key: " << key << std::endl;

        while (!key_exists(key)) {
            cv.wait(lock);
            std::cout << "[GET] Thread " << id << " woke up, checking for " << key << std::endl;
        }

        Value val = data[key];
        std::cout << "[GET] Thread " << id << " retrieved: " << key << " = " << val << std::endl;

        std::this_thread::yield();
        return val;
    }

    void print_all() {
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "\nCACHE CONTENTS" << std::endl;
        if (data.empty()) {
            std::cout << "Cache is empty" << std::endl;
        } else {
            for (const auto& p : data) {
                std::cout << "  " << p.first << " -> " << p.second << std::endl;
            }
        }
        std::cout << "*_________________*" << std::endl;
    }
};

int main() {
    srand(time(nullptr));
    
    Cache<std::string, int> cache;
    
    std::cout << "Cache Test with 3 Writers and 4 Readers" << std::endl;
    std::cout << "Writers will create keys: user0-user8" << std::endl;
    std::cout << "Readers will try to read random keys\n" << std::endl;

    std::vector<std::thread> threads;

    for (int i = 0; i < 3; ++i) {
        threads.push_back(std::thread([&cache, i]() {
            for (int j = 0; j < 3; ++j) {
                std::string key = "user" + std::to_string(i * 3 + j);
                int value = 1000 + (i * 100) + (j * 50);
                cache.set(key, value);
                std::this_thread::sleep_for(std::chrono::milliseconds(80));
            }
        }));
    }

    for (int i = 0; i < 4; ++i) {
        threads.push_back(std::thread([&cache, i]() {
            for (int j = 0; j < 4; ++j) {
                std::string key = "user" + std::to_string(rand() % 9);
                int val = cache.get(key);
                std::this_thread::sleep_for(std::chrono::milliseconds(40));
            }
        }));
    }

    for (auto& t : threads) {
        t.join();
    }

    cache.print_all();

    return 0;
}