#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <thread>

class VirtProc {
private:
    std::queue<std::string> jobs;
    std::string current;
    bool busy;
    
public:
    VirtProc(const std::vector<std::string>& j) : busy(false) {
        for (const auto& task : j) {
            jobs.push(task);
        }
    }
    
    bool isActive() const {
        return !jobs.empty() || busy;
    }
    
    void processNext(int pid) {
        if (!isActive()) return;
        
        if (!busy && !jobs.empty()) {
            current = jobs.front();
            jobs.pop();
            busy = true;
            std::cout << "Processor " << pid << " started job " << current << std::endl;
        }
        else if (busy) {
            busy = false;
            std::cout << "Processor " << pid << " finished job " << current << std::endl;
        }
    }
};

class HTSim {
private:
    VirtProc p1, p2;
    
public:
    HTSim(const std::vector<std::string>& a, const std::vector<std::string>& b) : p1(a), p2(b) {}
    
    void execute() {
        while (p1.isActive() || p2.isActive()) {
            if (p1.isActive()) p1.processNext(1);
            if (p2.isActive()) p2.processNext(2);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
};

int main() {
    std::vector<std::string> queue1 = {"X", "Z", "W"};
    std::vector<std::string> queue2 = {"Y", "U", "V"};
    
    HTSim sim(queue1, queue2);
    sim.execute();
    
    return 0;
}