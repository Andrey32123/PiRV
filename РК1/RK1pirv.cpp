//g++ RK1pirv.cpp -o program.exe -L C:\local\boost_1_87_0\stage\lib -lboost_thread -I C:\local\boost_1_87_0
#include <iostream>
#include <boost/thread.hpp>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>
#include <vector>
#include <atomic>
#include <chrono>
#include <random>
#include <iomanip>

void task1();
void task2();

int main() {

    std::cout << "task 1\n";
    task1();
    std::cout << "\ntask 2\n";
    task2();

    return 0;
}

//                             TASK 1
class MachineManager {
private:
    static const int MACHINES = 3;
    static const int WORKERS = 5;

    std::vector<bool> free_machine;
    boost::mutex mtx;
    boost::condition_variable cv;
    std::atomic<int> completed;
    std::atomic<bool> stop;

    void worker(int id) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 3);

        while (!stop) {
            int machine = -1;
            
            {
                boost::unique_lock<boost::mutex> lock(mtx);
                cv.wait(lock, [this]() {
                    for (int i = 0; i < MACHINES; i++)
                        if (free_machine[i]) return true;
                    return false;
                });
                
                for (int i = 0; i < MACHINES; i++) {
                    if (free_machine[i]) {
                        free_machine[i] = false;
                        machine = i;
                        break;
                    }
                }
            }
            
            std::cout << "Worker " << id << " start machine " << machine << std::endl;
            
            int time = dist(gen);
            boost::this_thread::sleep_for(boost::chrono::seconds(time));
            
            {
                boost::lock_guard<boost::mutex> lock(mtx);
                free_machine[machine] = true;
                completed++;
                std::cout << "Worker " << id << " finish machine " << machine 
                          << " (" << time << " sec)" << std::endl;
            }
            cv.notify_one();
        }
    }

public:
    MachineManager() : free_machine(MACHINES, true), completed(0), stop(false) {}
    
    void run(int seconds) {
        std::cout << "Machines: " << MACHINES << ", Workers: " << WORKERS << std::endl;
        
        boost::thread_group workers;
        for (int i = 0; i < WORKERS; i++)
            workers.create_thread(boost::bind(&MachineManager::worker, this, i));
        
        boost::this_thread::sleep_for(boost::chrono::seconds(seconds));
        stop = true;
        cv.notify_all();
        workers.join_all();
        
        std::cout << "\nTotal completed: " << completed << std::endl;
    }
};

void task1() {
    MachineManager m;
    m.run(10);
}

//                           TASK 2 
class StorageManager {
private:
    static const int TOTAL = 10;
    static const int MACHINES = 3;

    boost::interprocess::interprocess_semaphore machines{MACHINES};
    boost::interprocess::interprocess_semaphore workpieces{TOTAL};
    std::atomic<int> processed;

    void worker(int id) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(1, 2);

        while (processed < TOTAL) {
            if (!workpieces.try_wait()) break;
            
            machines.wait();
            
            if (processed >= TOTAL) {
                machines.post();
                workpieces.post();
                break;
            }
            
            int time = dist(gen);
            std::cout << "Worker " << id << " loaded, processing " << time << " sec" << std::endl;
            
            boost::this_thread::sleep_for(boost::chrono::seconds(time));
            
            processed++;
            std::cout << "Processed: " << processed << "/" << TOTAL << std::endl;
            
            machines.post();
        }
    }

public:
    StorageManager() : processed(0) {}
    
    void run() {
        std::cout << "Workpieces: " << TOTAL << ", Machines: " << MACHINES << std::endl;
        
        auto start = std::chrono::high_resolution_clock::now();
        
        boost::thread_group workers;
        for (int i = 0; i < 5; i++)
            workers.create_thread(boost::bind(&StorageManager::worker, this, i));
        
        workers.join_all();
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
        
        std::cout << "\nTotal processed: " << processed << std::endl;
        std::cout << "Time: " << duration.count() << " sec" << std::endl;
    }
};

void task2() {
    StorageManager s;
    s.run();
}