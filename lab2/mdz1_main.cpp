//g++ -std=c++20 lab2_main.cpp -o program.exe

#include <iostream>
#include <thread>
#include <mutex>
#include <semaphore>
#include <queue>
#include <vector>
#include <random>
#include <chrono>
#include <atomic>

using namespace std;
using namespace chrono;

//1

class Port {
public:
    int INIT_CRANES = 5;
    int MAX_CRANES = 6;
    int NUM_TRUCKS = 10;
    int QUEUE_LIMIT = 5;
    int EMERGENCY_LIMIT = 3;
    
    counting_semaphore<6> cranes{5};
    mutex mtx;
    atomic<int> loaded{0};
    atomic<int> waiting{0};
    atomic<bool> emergency{false};
    atomic<bool> backup{false};
    
    random_device rd;
    mt19937 gen{rd()};
    
    int getTime() {
        if (emergency) {
            uniform_int_distribution<> dis(1, 3);
            return dis(gen);
        } else {
            uniform_int_distribution<> dis(3, 6);
            return dis(gen);
        }
    }
    
    void truck(int id) {
        waiting++;
        
        {
            lock_guard<mutex> lock(mtx);
            cout << "[TRUCK " << id << "] Arrived. Queue: " << waiting.load() << endl;
        }
        
        cranes.acquire();
        waiting--;
        
        if (loaded < EMERGENCY_LIMIT && emergency == false) {
            emergency = true;
            cout << "[EMERGENCY] Speed mode ON (1-3 sec)" << endl;
        }
        
        if (waiting > QUEUE_LIMIT && backup == false) {
            backup = true;
            cout << "[SYSTEM] Backup crane activated!" << endl;
        }
        
        int time = getTime();
        
        {
            lock_guard<mutex> lock(mtx);
            cout << "[TRUCK " << id << "] Loading... " << time << " sec" << endl;
        }
        
        this_thread::sleep_for(seconds(time));
        loaded++;
        
        {
            lock_guard<mutex> lock(mtx);
            cout << "[TRUCK " << id << "] Done! Total: " << loaded.load() << endl;
        }
        
        cranes.release();
    }
    
    void run() {
        cout << "\n PORT SYSTEM" << endl;
        vector<thread> trucks;
        
        for (int i = 1; i <= NUM_TRUCKS; i++) {
            trucks.push_back(thread(&Port::truck, this, i));
            this_thread::sleep_for(milliseconds(300));
        }
        
        for (int i = 0; i < trucks.size(); i++) {
            trucks[i].join();
        }
        
        cout << "COMPLETED: " << loaded.load() << " trucks" << endl;
    }
};

// 2

class VideoSystem {
public:
    int CAMERAS = 6;
    int ACCELERATORS = 3;
    int FRAMES = 8;
    int TOTAL_FRAMES = 48;
    
    struct Frame {
        int camera_id;
        int frame_number;
        int processing_time;
        int is_important;
    };
    
    queue<Frame> q;
    
    counting_semaphore<100> full{0};
    counting_semaphore<100> empty{100};
    
    counting_semaphore<1> acc1{1};
    counting_semaphore<1> acc2{1};
    counting_semaphore<1> acc3{1};
    
    mutex queue_mutex;
    mutex cout_mutex;
    
    atomic<int> processed{0};
    atomic<int> running{1};
    atomic<int> failed{0};
    atomic<bool> failure_reported{false};
    
    random_device rd;
    mt19937 gen{rd()};
    
    counting_semaphore<1>& getAccSem(int id) {
        if (id == 1) return acc1;
        if (id == 2) return acc2;
        return acc3;
    }
    
    void monitor() {
        while (running == 1) {
            this_thread::sleep_for(seconds(3));
            
 
            if (failed == 0) {
                int r = rand() % 100;
                if (r < 15) {
                    int f = (rand() % ACCELERATORS) + 1;
                    failed = f;
                    failure_reported = false;
                    cout << "[FAILURE] Accelerator " << failed << " failed!" << endl;
                }
            }
            
 
            if (failed > 0) {
                int r = rand() % 100;
                if (r < 30) {
                    cout << "[RECOVERY] Accelerator " << failed << " recovered!" << endl;
                    failed = 0;
                    failure_reported = false;
                }
            }
        }
    }
    
    void camera(int id) {
        for (int f = 1; f <= FRAMES; f++) {
            empty.acquire();
            
            Frame frame;
            frame.camera_id = id;
            frame.frame_number = f;
            
            if (f % 2 == 0) {
                frame.is_important = 1;
                uniform_int_distribution<> dis(3, 5);
                frame.processing_time = dis(gen);
            } else {
                frame.is_important = 0;
                uniform_int_distribution<> dis(1, 3);
                frame.processing_time = dis(gen);
            }
            
            {
                lock_guard<mutex> lock(queue_mutex);
                q.push(frame);
            }
            
            full.release();
            
            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "[CAMERA " << id << "] Frame " << f;
                if (frame.is_important == 1) cout << " [IMPORTANT]";
                cout << " queued" << endl;
            }
            
            this_thread::sleep_for(milliseconds(500));
        }
    }
    
    void accelerator(int id) {
        while (running == 1) {
            full.acquire();
            
            if (running == 0 && q.empty()) {
                full.release();
                break;
            }
            
            Frame frame;
            {
                lock_guard<mutex> lock(queue_mutex);
                if (q.empty() == false) {
                    frame = q.front();
                    q.pop();
                } else {
                    full.release();
                    continue;
                }
            }
            
            empty.release();
            
            if (running == 0) {
                break;
            }
            

            if (id == failed) {

                if (failure_reported == false) {
                    failure_reported = true;
                    lock_guard<mutex> lock(cout_mutex);
                    cout << "[ACC " << id << "] FAILED! Frames will be rerouted silently" << endl;
                }
                
                empty.acquire();
                {
                    lock_guard<mutex> lock(queue_mutex);
                    q.push(frame);
                }
                full.release();
                continue;
            }
            
            getAccSem(id).acquire();
            
            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "[ACC " << id << "] Processing: Camera " << frame.camera_id;
                cout << " Frame " << frame.frame_number;
                if (frame.is_important == 1) cout << " [IMP]";
                cout << " " << frame.processing_time << " sec" << endl;
            }
            
            this_thread::sleep_for(seconds(frame.processing_time));
            
            if (processed < TOTAL_FRAMES) {
                processed++;
            }
            
            {
                lock_guard<mutex> lock(cout_mutex);
                cout << "[ACC " << id << "] Completed! Total: " << processed.load();
                cout << "/" << TOTAL_FRAMES << endl;
            }
            
            getAccSem(id).release();
        }
    }
    
    void run() {
        cout << "\nVIDEO SYSTEM" << endl;
        
        vector<thread> cameras;
        vector<thread> accelerators;
        
        thread monitor_th(&VideoSystem::monitor, this);
        
        for (int i = 1; i <= CAMERAS; i++) {
            cameras.push_back(thread(&VideoSystem::camera, this, i));
        }
        
        for (int i = 1; i <= ACCELERATORS; i++) {
            accelerators.push_back(thread(&VideoSystem::accelerator, this, i));
        }
        
        for (int i = 0; i < cameras.size(); i++) {
            cameras[i].join();
        }
        
        cout << "[SYSTEM] All cameras finished. Waiting for processing..." << endl;
        
        while (processed < TOTAL_FRAMES) {
            this_thread::sleep_for(milliseconds(100));
        }
        
        running = 0;
        
        for (int i = 0; i < ACCELERATORS; i++) {
            full.release();
        }
        
        for (int i = 0; i < accelerators.size(); i++) {
            accelerators[i].join();
        }
        
        monitor_th.join();
        
        cout << "COMPLETED: " << processed.load() << "/" << TOTAL_FRAMES << " frames" << endl;
    }
};

int main() {
    cout << "LAB2" << endl;
    
    Port port;
    port.run();
    
    VideoSystem video;
    video.run();
    
    cout << "\nEND" << endl;
    
    return 0;
}