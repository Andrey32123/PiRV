//g++ mdz1_main.cpp -o program.exe -L C:\local\boost_1_87_0\stage\lib -lboost_thread -I C:\local\boost_1_87_0
#include <boost/thread.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <atomic>
#include <chrono>
#include <iomanip>

struct FileResult {
    std::string filename;
    size_t word_count;
    size_t char_count;
    size_t line_count;
    bool success;
    std::string error_message;

    FileResult() : word_count(0), char_count(0), line_count(0), success(false) {}
};

class FileProcessor {
private:
    std::vector<std::string> input_files;
    std::vector<FileResult> results;
    boost::mutex results_mutex;
    boost::mutex output_mutex;
    boost::condition_variable completion_cv;
    std::atomic<int> processed_count;
    int total_files;

    FileResult process_single_file(const std::string& filename) {
        FileResult result;
        result.filename = filename;

        std::ifstream file(filename.c_str());
        if (!file.is_open()) {
            result.success = false;
            result.error_message = "Cannot open file";
            return result;
        }

        std::string line;
        size_t char_count = 0;
        size_t word_count = 0;
        size_t line_count = 0;

        while (std::getline(file, line)) {
            line_count++;
            char_count += line.length() + 1;

            bool in_word = false;
            for (size_t i = 0; i < line.length(); ++i) {
                char ch = line[i];
                if (std::isspace(static_cast<unsigned char>(ch))) {
                    if (in_word) {
                        word_count++;
                        in_word = false;
                    }
                }
                else {
                    in_word = true;
                }
            }
            if (in_word) {
                word_count++;
            }
        }

        result.char_count = char_count;
        result.word_count = word_count;
        result.line_count = line_count;
        result.success = true;

        std::string output_filename = "processed_" + filename;
        std::ofstream output_file(output_filename.c_str());
        if (output_file.is_open()) {
            output_file << "Processing results for " << filename  << std::endl;
            output_file << "Characters: " << char_count << std::endl;
            output_file << "Words: " << word_count << std::endl;
            output_file << "Lines: " << line_count << std::endl;
            output_file.close();
        }

        return result;
    }

    void worker_thread(int thread_id, const std::string& filename) {
        {
            boost::lock_guard<boost::mutex> lock(output_mutex);
            std::cout << "[Thread " << thread_id << "] Started processing: "
                      << filename << std::endl;
        }

        auto start = std::chrono::high_resolution_clock::now();

        FileResult result = process_single_file(filename);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        {
            boost::lock_guard<boost::mutex> lock(results_mutex);
            results[thread_id] = result;
        }

        {
            boost::lock_guard<boost::mutex> lock(output_mutex);
            std::cout << "[Thread " << thread_id << "] Finished processing: "
                      << filename << " (time: " << duration.count() << " microseconds)" << std::endl;
        }

        processed_count++;
        completion_cv.notify_one();
    }

public:
    FileProcessor(const std::vector<std::string>& files)
        : input_files(files),
          results(files.size()),
          processed_count(0),
          total_files(static_cast<int>(files.size())) {}

    void run() {
        std::cout << "Total files to process: " << total_files << std::endl;

        auto global_start = std::chrono::high_resolution_clock::now();

        boost::thread_group threads;

        for (int i = 0; i < total_files; ++i) {
            threads.create_thread(boost::bind(&FileProcessor::worker_thread, this, i, input_files[i]));
        }

        {
            boost::unique_lock<boost::mutex> lock(results_mutex);
            completion_cv.wait(lock, [this]() {
                return processed_count.load() == total_files;
            });
        }

        threads.join_all();

        auto global_end = std::chrono::high_resolution_clock::now();
        auto global_duration = std::chrono::duration_cast<std::chrono::microseconds>(global_end - global_start);

        std::cout << "\nITOGO\n" << std::endl;
        for (size_t i = 0; i < results.size(); ++i) {
            const FileResult& result = results[i];
            std::cout << "File: " << result.filename << std::endl;
            std::cout << "  Characters: " << result.char_count << std::endl;
            std::cout << "  Words: " << result.word_count << std::endl;
            std::cout << "  Lines: " << result.line_count << std::endl;
            if (!result.success) {
                std::cout << "  Error: " << result.error_message << std::endl;
            }
            std::cout << std::endl;
        }

        std::cout << "Processed files count: " << processed_count.load() << std::endl;
        std::cout << "Total execution time: " << global_duration.count() << " microseconds" << std::endl;
    }
};

void create_test_files() {
    struct TestFile {
        std::string name;
        std::string content;
    };

    TestFile test_files[] = {
        {"data1.txt",
         "First file.\n"
         "Hello world!\n"
         "Another hello world\n"
         "And another hello_world\n"},

        {"data2.txt",
         "Second file.\nYa sluchayno sdelal\ndz i rk 1 variant\npo variantu lab\nne beyte pls\nLast line\n"},

        {"data3.txt",
         "Third file.\n"
         "Lorem ipsum dolor sit amet consectetur adipiscing elit. Quisque faucibus ex sapien vitae pellentesque sem placerat. In id cursus mi pretium tellus duis convallis. Tempus leo eu aenean sed diam urna tempor. Pulvinar vivamus fringilla lacus nec metus bibendum egestas. Iaculis massa nisl malesuada lacinia integer nunc posuere. Ut hendrerit semper vel class aptent taciti sociosqu. Ad litora torquent per conubia nostra inceptos himenaeos.\n"}
    };

    for (int i = 0; i < 3; ++i) {
        std::ofstream file(test_files[i].name.c_str());
        if (file.is_open()) {
            file << test_files[i].content;
            file.close();
            std::cout << "Created test file: " << test_files[i].name << std::endl;
        }
    }
}

int main() {
    create_test_files();
    std::cout << std::endl;

    std::vector<std::string> files;
    files.push_back("data1.txt");
    files.push_back("data2.txt");
    files.push_back("data3.txt");

    FileProcessor processor(files);
    processor.run();

    return 0;
}