#include <iostream>
#include <vector>
#include <chrono>
#include <boost/thread.hpp>

// Рекурсивное вычисление чисел Фибоначчи
long long fibonacci(int n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void task(int n,int id){
    long long res=fibonacci(n);
    std::cout<<"thread "<<id<<" end calculating "<<n<<" fibonacci, result: "<<res<<'\n';
}



int main() {
    int a[]={37,38,39,40,41,42,43,44};
    //последовательно
    auto start = std::chrono::high_resolution_clock::now();
    for (int i =0;i<8;i++){
        std::cout<<a[i]<<" fibonacci is "<<fibonacci(a[i])<<'\n';
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout<<"time for calculating: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<<"ms"<<'\n'<<'\n';
    
    
    

    //2 потока
    std::vector<boost::thread> threads2;
    std::cout<<"calculating with 2 threads: \n";
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 8; i+=2) {
        threads2.clear();
        threads2.emplace_back(task, a[i],1);
        threads2.emplace_back(task, a[i+1],2);
        for (auto& t:threads2){
            t.join();
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout<<"time for calculating with 2 threads: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<<"ms"<<'\n'<<'\n';

    //4 потока
    std::vector<boost::thread> threads4;
    std::cout<<"calculating with 4 threads: \n";
    start = std::chrono::high_resolution_clock::now(); 
    for (int i = 0; i < 8; i+=4) {
        threads4.clear();
        threads4.emplace_back(task, a[i],1);
        threads4.emplace_back(task, a[i+1],2);
        threads4.emplace_back(task, a[i+2],3);
        threads4.emplace_back(task, a[i+3],4);
        for (auto& t:threads4){
            t.join();
        }
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout<<"time for calculating with 4 threads: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<<"ms"<<'\n'<<'\n';

    //8 потоков
    std::vector<boost::thread> threads8;
    std::cout<<"calculating with 8 threads: \n";
    start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 8; i++) {
        threads8.emplace_back(task, a[i],i+1);    
    }
    for (auto& t:threads8){
        t.join();
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout<<"time for calculating with 8 threads: "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()<<"ms"<<'\n'<<'\n';

    
    return 0;
}