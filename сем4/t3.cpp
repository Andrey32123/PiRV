// task3
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <chrono>

template<typename T>
class Buffer {
private:
    std::vector<T> buffer;
    size_t capacity;
    std::mutex mtx;
    std::condition_variable not_full;
    std::condition_variable not_empty;
    int produced_count;
    int consumed_count;

public:
    Buffer(size_t cap) : capacity(cap), produced_count(0), consumed_count(0) {
        buffer.reserve(capacity);
    }

    void produce(T value, int producer_id, int max_items) {
        std::unique_lock<std::mutex> lock(mtx);
        auto thread_id = std::this_thread::get_id();

        while (buffer.size() >= capacity) {
            std::cout << "Producer " << producer_id << " [" << thread_id
                      << "] waiting - buffer full" << std::endl;
            not_full.wait(lock);
            std::this_thread::yield();
        }

        buffer.push_back(value);
        produced_count++;
        std::cout << "Producer " << producer_id << " [" << thread_id
                  << "] produced: " << value << " (buffer: " << buffer.size()
                  << "/" << capacity << ", total: " << produced_count << ")" << std::endl;

        not_empty.notify_one();
        std::this_thread::yield();
    }

    T consume(int consumer_id) {
        std::unique_lock<std::mutex> lock(mtx);
        auto thread_id = std::this_thread::get_id();

        while (buffer.empty()) {
            std::cout << "Consumer " << consumer_id << " [" << thread_id
                      << "] waiting - buffer empty" << std::endl;
            not_empty.wait(lock);
            std::this_thread::yield();
        }

        T value = buffer.back();
        buffer.pop_back();
        consumed_count++;
        std::cout << "Consumer " << consumer_id << " [" << thread_id
                  << "] consumed: " << value << " (buffer: " << buffer.size()
                  << "/" << capacity << ", total: " << consumed_count << ")" << std::endl;

        not_full.notify_one();
        std::this_thread::yield();

        return value;
    }
};

int main() {
    srand(time(nullptr));
    
    Buffer<int> buffer(4);
    
    std::cout << "Buffer capacity: 4" << std::endl;
    std::cout << "Producers: 3, Consumers: 2" << std::endl;
    std::cout << "Each producer: 5 items, Each consumer: 7 items (some will wait)\n" << std::endl;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    for (int i = 0; i < 3; ++i) {
        producers.push_back(std::thread([&buffer, i]() {
            for (int j = 0; j < 5; ++j) {
                int value = (i + 1) * 100 + j * 10;
                buffer.produce(value, i, 5);
                std::this_thread::sleep_for(std::chrono::milliseconds(20 + rand() % 40));
            }
        }));
    }

    for (int i = 0; i < 2; ++i) {
        consumers.push_back(std::thread([&buffer, i]() {
            for (int j = 0; j < 7; ++j) {
                buffer.consume(i);
                std::this_thread::sleep_for(std::chrono::milliseconds(30 + rand() % 50));
            }
        }));
    }

    for (auto& t : producers) t.join();
    for (auto& t : consumers) t.join();

    std::cout << "\nAll threads completed" << std::endl;

    return 0;
}