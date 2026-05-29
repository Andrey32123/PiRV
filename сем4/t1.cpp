// task1
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>

template<typename T>
class ParallelSum {
private:
    std::vector<T> data;
    size_t num_threads;
    T total_sum;
    std::mutex sum_mutex;
    std::condition_variable cv;
    size_t completed_threads;

    inline T sum_segment(typename std::vector<T>::iterator start,
        typename std::vector<T>::iterator end) {
        T sum = 0;
        for (auto it = start; it != end; ++it) {
            sum += *it;
            if ((it - start) % 10 == 0) std::this_thread::yield();
        }
        return sum;
    }

public:
    ParallelSum(const std::vector<T>& vec, size_t threads)
        : data(vec), num_threads(threads), total_sum(0), completed_threads(0) {
        if (num_threads == 0) num_threads = 1;
    }

    T compute_sum() {
        size_t segment_size = data.size() / num_threads;
        size_t remainder = data.size() % num_threads;
        auto start = data.begin();

        for (size_t i = 0; i < num_threads; ++i) {
            size_t current_size = segment_size + (i < remainder ? 1 : 0);
            auto end = start + current_size;

            std::thread t([this, start, end, i]() {
                T segment_sum = sum_segment(start, end);

                {
                    std::lock_guard<std::mutex> lock(sum_mutex);
                    std::cout << "Thread " << std::this_thread::get_id()
                        << " segment " << (i + 1) << " sum: " << segment_sum << std::endl;
                    total_sum += segment_sum;
                    completed_threads++;
                }
                cv.notify_one();
            });
            t.detach();
            start = end;
        }

        std::unique_lock<std::mutex> lock(sum_mutex);
        cv.wait(lock, [this]() { return completed_threads == num_threads; });

        std::cout << "Final sum: " << total_sum << std::endl;
        return total_sum;
    }
};

int main() {
    std::vector<double> data;
    for (int i = 1; i <= 25; i++) {
        data.push_back(i * 1.5);
    }

    ParallelSum<double> ps(data, 5);
    double result = ps.compute_sum();

    double expected = 0;
    for (int i = 1; i <= 25; i++) expected += i * 1.5;
    std::cout << "Result: " << result << " (expected: " << expected << ")" << std::endl;

    return 0;
}