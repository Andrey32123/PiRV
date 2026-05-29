#include <iostream>
#include <vector>

long long fact(int n) {
    long long res = 1;
    for (int i = 2; i <= n; ++i) {
        res *= i;
    }
    return res;
}

class VirtThread {
private:
    std::vector<int> tasks;
    size_t idx;
    
public:
    VirtThread(const std::vector<int>& t) : tasks(t), idx(0) {}
    
    bool hasWork() const {
        return idx < tasks.size();
    }
    
    void doWork(int threadNum) {
        if (hasWork()) {
            int val = tasks[idx++];
            long long res = fact(val);
            std::cout << "Thread " << threadNum << " computing " << val << "! = " << res << std::endl;
        }
    }
};

class HTSimulator {
private:
    VirtThread t1, t2;
    
public:
    HTSimulator(const std::vector<int>& a, const std::vector<int>& b) : t1(a), t2(b) {}
    
    void run() {
        while (t1.hasWork() || t2.hasWork()) {
            if (t1.hasWork()) t1.doWork(1);
            if (t2.hasWork()) t2.doWork(2);
        }
    }
};

int main() {
    std::vector<int> firstBatch = {5, 10, 15};
    std::vector<int> secondBatch = {7, 12, 8};
    
    HTSimulator sim(firstBatch, secondBatch);
    sim.run();
    
    return 0;
}