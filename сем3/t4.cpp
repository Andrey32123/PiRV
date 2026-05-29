#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

class Stopwatch {
private:
    std::chrono::high_resolution_clock::time_point t0;
    std::chrono::high_resolution_clock::time_point t1;
    
public:
    void start() {
        t0 = std::chrono::high_resolution_clock::now();
    }
    
    void stop() {
        t1 = std::chrono::high_resolution_clock::now();
    }
    
    long long getMillis() const {
        return std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    }
    
    void measureSort(std::vector<int>& data) {
        start();
        std::sort(data.begin(), data.end());
        stop();
        std::cout << "Sort duration: " << getMillis() << " ms" << std::endl;
    }
};

std::vector<int> makeRandomVector(size_t sz) {
    std::vector<int> vec(sz);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 1000000);
    
    for (auto& val : vec) {
        val = dist(gen);
    }
    return vec;
}

int main() {
    const size_t SIZE = 100000;
    Stopwatch timer;
    
    std::vector<int> firstArr = makeRandomVector(SIZE);
    timer.measureSort(firstArr);
    
    std::vector<int> secondArr = makeRandomVector(SIZE);
    timer.measureSort(secondArr);
    
    return 0;
}