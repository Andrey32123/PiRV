#include <iostream>
#include <chrono>

int main() {
    long long secs;
    
    std::cout << "Input seconds: ";
    std::cin >> secs;
    
    std::chrono::seconds total(secs);
    
    auto hrs = std::chrono::duration_cast<std::chrono::hours>(total);
    auto afterHrs = total - hrs;
    
    auto mins = std::chrono::duration_cast<std::chrono::minutes>(afterHrs);
    auto remSecs = afterHrs - mins;
    
    std::cout << hrs.count() << " h ";
    std::cout << mins.count() << " min ";
    std::cout << remSecs.count() << " sec" << std::endl;
    
    return 0;
}