// task7
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

template<typename T>
class PriorityQueue {
private:
    struct Item {
        T value;
        int priority;
        long long sequence;

        bool operator<(const Item& other) const {
            if (priority != other.priority) {
                return priority < other.priority;
            }
            return sequence > other.sequence;
        }
    };

    std::priority_queue<Item> queue;
    std::mutex mtx;
    std::condition_variable cv;
    std::mutex cout_mtx;
    long long next_seq;

public:
    PriorityQueue() : next_seq(0) {}

    void push(T value, int priority) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            auto id = std::this_thread::get_id();

            queue.push({value, priority, next_seq++});

            {
                std::lock_guard<std::mutex> cout_lock(cout_mtx);
                std::cout << "[Producer " << id << "] PUSH: " << value
                          << " (priority: " << priority << ")"
                          << " [queue size: " << queue.size() << "]" << std::endl;
            }

            cv.notify_one();
        }
        std::this_thread::yield();
    }

    T pop() {
        std::unique_lock<std::mutex> lock(mtx);
        auto id = std::this_thread::get_id();

        cv.wait(lock, [this]() { return !queue.empty(); });

        Item item = queue.top();
        queue.pop();

        {
            std::lock_guard<std::mutex> cout_lock(cout_mtx);
            std::cout << "[Consumer " << id << "] POP: " << item.value
                      << " (priority: " << item.priority << ")"
                      << " [queue size: " << queue.size() << "]" << std::endl;
        }

        std::this_thread::yield();
        return item.value;
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(mtx);
        return queue.size();
    }
};

int main() {
    PriorityQueue<std::string> pq;
    
    std::cout << "Priority queue test" << std::endl;
    std::cout << "Priorities: 1=Low, 5=Medium, 9=High, 10=Urgent\n" << std::endl;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    consumers.push_back(std::thread([&pq]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        for (int i = 0; i < 12; ++i) {
            pq.pop();
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
    }));

    producers.push_back(std::thread([&pq]() {
        pq.push("Task alpha", 3);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        pq.push("Task beta", 3);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        pq.push("Task gamma", 8);
    }));

    producers.push_back(std::thread([&pq]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        pq.push("Task delta", 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        pq.push("Task epsilon", 5);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        pq.push("Task zeta", 10);
    }));

    producers.push_back(std::thread([&pq]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
        pq.push("Task eta", 7);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        pq.push("Task theta", 2);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        pq.push("Task iota", 9);
    }));

    for (auto& t : producers) {
        t.join();
    }

    std::this_thread::sleep_for(std::chrono::seconds(2));

    for (auto& t : consumers) {
        t.detach();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "\nFinal queue size: " << pq.size() << std::endl;
    std::cout << "Test completed" << std::endl;

    return 0;
}