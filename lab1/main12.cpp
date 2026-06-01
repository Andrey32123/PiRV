#include <iostream>

#include <boost/thread.hpp>
#include <mutex>
#include <atomic>
#include <chrono>
std::mutex mtx;

void incr(int& cntr){
    for (int i=0;i<500000;i++){
        cntr++;
    }
}
void Mtx(int& cntr){
    
    for (int i=0;i<500000;i++){
        std::lock_guard<std::mutex> lock(mtx);
        cntr++;
    }
}
void Atmc(std::atomic<int>& cntr){
    for (int i=0;i<500000;i++){
        cntr++;
    }
}

int main(){
    //гонка данных
    int counter1=0;
    auto start = std::chrono::high_resolution_clock::now();
    boost::thread t11(incr,std::ref(counter1));
    boost::thread t12(incr,std::ref(counter1));
    t11.join();
    t12.join();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout<<"res without synchronization: "<<counter1<<". time to incr: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<<"ms"<<'\n'<<'\n';

    //mutex
    int counter2=0;
    start = std::chrono::high_resolution_clock::now();
    boost::thread t21(Mtx,std::ref(counter2));
    boost::thread t22(Mtx,std::ref(counter2));
    t21.join();
    t22.join();
    end = std::chrono::high_resolution_clock::now();
    std::cout<<"res with mutex: "<<counter2<<". time to incr: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<<"ms"<<'\n'<<'\n';
    
    //atomic
    std::atomic<int> counter3(0);
    start = std::chrono::high_resolution_clock::now();
    boost::thread t31(Atmc,std::ref(counter3));
    boost::thread t32(Atmc,std::ref(counter3));
    t31.join();
    t32.join();
    end = std::chrono::high_resolution_clock::now();
    std::cout<<"res with atomic: "<<counter3<<". time to incr: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<<"ms"<<'\n'<<'\n';
    
    return 0;
}