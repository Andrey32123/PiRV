#include <iostream>
#include <chrono>
#include <thread>

int main() {
    int countdown;
    
    std::cout << "Enter countdown seconds: ";
    std::cin >> countdown;
    
    while (countdown > 0) {
        std::cout << "Remaining: " << countdown << " sec" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        --countdown;
    }
    
    std::cout << "Time's up!" << std::endl;
    
    return 0;
}