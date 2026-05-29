#include <iostream>
#include <chrono>

long long calcSum(long long maxVal) {
    long long total = 0;
    for (long long i = 1; i <= maxVal; ++i) {
        total += i;
    }
    return total;
}

int main() {
    const long long LIMIT = 100000000;

    auto begin = std::chrono::high_resolution_clock::now();
    
    long long answer = calcSum(LIMIT);
    
    auto end = std::chrono::high_resolution_clock::now();
    
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    
    std::cout << "Sum 1.." << LIMIT << " = " << answer << std::endl;
    std::cout << "Execution time: " << elapsed.count() << " ms" << std::endl;
    
    return 0;
}