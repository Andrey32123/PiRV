// task6
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <functional>

template<typename T>
class MatrixProcessor {
private:
    std::vector<std::vector<T>> matrix;
    size_t rows;
    size_t cols;
    size_t num_threads;
    std::mutex cout_mtx;
    std::mutex counter_mtx;
    int processed_elements;

    inline void process_segment(size_t start_row, size_t end_row, 
                                 const std::function<T(T)>& func, int thread_id) {
        auto id = std::this_thread::get_id();
        int local_processed = 0;

        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << "[Thread " << id << "] started rows " << start_row 
                      << " to " << (end_row - 1) << std::endl;
        }

        for (size_t r = start_row; r < end_row; ++r) {
            for (size_t c = 0; c < cols; ++c) {
                matrix[r][c] = func(matrix[r][c]);
                local_processed++;
                
                if (local_processed % 5 == 0) {
                    std::this_thread::yield();
                }
            }
            std::this_thread::yield();
        }

        {
            std::lock_guard<std::mutex> lock(counter_mtx);
            processed_elements += local_processed;
        }

        {
            std::lock_guard<std::mutex> lock(cout_mtx);
            std::cout << "[Thread " << id << "] finished (processed " 
                      << local_processed << " elements)" << std::endl;
        }
    }

public:
    MatrixProcessor(const std::vector<std::vector<T>>& mat, size_t threads)
        : matrix(mat), num_threads(threads), processed_elements(0) {
        rows = matrix.size();
        cols = (rows > 0) ? matrix[0].size() : 0;

        if (num_threads == 0) num_threads = 1;
        if (num_threads > rows) num_threads = rows;
    }

    void apply(const std::function<T(T)>& func) {
        std::vector<std::thread> threads;
        processed_elements = 0;

        size_t rows_per_thread = rows / num_threads;
        size_t remainder = rows % num_threads;
        size_t start_row = 0;

        std::cout << "\nProcessing " << rows << "x" << cols 
                  << " matrix with " << num_threads << " threads" << std::endl;

        for (size_t i = 0; i < num_threads; ++i) {
            size_t current_rows = rows_per_thread + (i < remainder ? 1 : 0);
            size_t end_row = start_row + current_rows;
            
            threads.push_back(std::thread(&MatrixProcessor::process_segment, this,
                start_row, end_row, func, i));

            start_row = end_row;
        }

        for (auto& t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }

        std::cout << "Total elements processed: " << processed_elements 
                  << " / " << (rows * cols) << std::endl;
    }

    void print_matrix(const std::string& title) {
        std::lock_guard<std::mutex> lock(cout_mtx);
        std::cout << "\n" << title << ":" << std::endl;
        for (const auto& row : matrix) {
            for (T val : row) {
                std::cout << val << "\t";
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    std::vector<std::vector<int>> mat = {
        {2, 4, 6, 8, 10},
        {12, 14, 16, 18, 20},
        {22, 24, 26, 28, 30},
        {32, 34, 36, 38, 40},
        {42, 44, 46, 48, 50},
        {52, 54, 56, 58, 60}
    };

    MatrixProcessor<int> mp(mat, 4);
    mp.print_matrix("Original Matrix");

    mp.apply([](int x) { return x * 2; });
    mp.print_matrix("After multiply by 2");

    mp.apply([](int x) { return x - 5; });
    mp.print_matrix("After subtract 5");

    mp.apply([](int x) { return x / 3; });
    mp.print_matrix("After divide by 3");

    return 0;
}