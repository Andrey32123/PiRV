#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>

class Job {
public:
    int number;
    int priority;
    int totalMs;
    int stepCount;
    int stepDone;
    
    Job(int n, int p, int ms, int steps)
        : number(n), priority(p), totalMs(ms), stepCount(steps), stepDone(0) {}
    
    bool finished() const {
        return stepDone >= stepCount;
    }
    
    int stepDuration() const {
        return totalMs / stepCount;
    }
    
    int square() const {
        return number * number;
    }
};

class VirtCore {
private:
    std::vector<Job> jobs;
    
public:
    void addJob(const Job& j) {
        jobs.push_back(j);
    }
    
    bool hasJobs() const {
        return !jobs.empty();
    }
    
    void execStep(int coreId) {
        if (jobs.empty()) return;
        
        auto best = std::max_element(jobs.begin(), jobs.end(),
            [](const Job& a, const Job& b) {
                return a.priority < b.priority;
            });
        
        Job& task = *best;
        task.stepDone++;
        
        int stepMs = task.stepDuration();
        
        std::cout << "Core " << coreId << " step " << task.stepDone << "/" << task.stepCount
                  << " of job " << task.number << " (priority " << task.priority << ")" << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(stepMs));
        
        if (task.finished()) {
            int result = task.square();
            std::cout << "Core " << coreId << " completed job " << task.number
                      << " -> result = " << result << std::endl;
            jobs.erase(best);
        }
    }
};

class HyperSim {
private:
    VirtCore v1, v2;
    
public:
    HyperSim(VirtCore a, VirtCore b) : v1(a), v2(b) {}
    
    void start() {
        while (v1.hasJobs() || v2.hasJobs()) {
            if (v1.hasJobs()) v1.execStep(1);
            if (v2.hasJobs()) v2.execStep(2);
        }
    }
};

int randRange(int low, int high, std::mt19937& gen) {
    std::uniform_int_distribution<int> dist(low, high);
    return dist(gen);
}

int main() {
    std::random_device rd;
    std::mt19937 rng(rd());
    
    VirtCore coreA, coreB;
    
    int taskCount1 = randRange(2, 4, rng);
    std::cout << " Generating " << taskCount1 << " tasks A" << std::endl;
    for (int i = 0; i < taskCount1; ++i) {
        int val = randRange(1, 50, rng);
        int prio = randRange(1, 10, rng);
        int dur = randRange(200, 1000, rng);
        int st = randRange(2, 5, rng);
        
        coreA.addJob(Job(val, prio, dur, st));
        std::cout << "  Job: num=" << val << ", prio=" << prio 
                  << ", duration=" << dur << ", steps=" << st << std::endl;
    }
    
    int taskCount2 = randRange(2, 4, rng);
    std::cout << "\nGenerating " << taskCount2 << " tasks B" << std::endl;
    for (int i = 0; i < taskCount2; ++i) {
        int val = randRange(1, 50, rng);
        int prio = randRange(1, 10, rng);
        int dur = randRange(200, 1000, rng);
        int st = randRange(2, 5, rng);
        
        coreB.addJob(Job(val, prio, dur, st));
        std::cout << "  Job: num=" << val << ", prio=" << prio 
                  << ", duration=" << dur << ", steps=" << st << std::endl;
    }
    
    std::cout << "\nStarting hyper-threading simulation\n" << std::endl;
    
    HyperSim simulator(coreA, coreB);
    simulator.start();
    
    std::cout << "\Completed" << std::endl;
    
    return 0;
}