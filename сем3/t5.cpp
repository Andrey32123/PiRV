#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

void bubbleSort(std::vector<int>& a) {
    int n = a.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (a[j] > a[j + 1]) {
                std::swap(a[j], a[j + 1]);
            }
        }
    }
}

void insertionSort(std::vector<int>& a) {
    int n = a.size();
    for (int i = 1; i < n; ++i) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && a[j] > key) {
            a[j + 1] = a[j];
            --j;
        }
        a[j + 1] = key;
    }
}

void merge(std::vector<int>& a, int l, int m, int r) {
    std::vector<int> left(a.begin() + l, a.begin() + m + 1);
    std::vector<int> right(a.begin() + m + 1, a.begin() + r + 1);
    
    int i = 0, j = 0, k = l;
    while (i < left.size() && j < right.size()) {
        if (left[i] <= right[j]) a[k++] = left[i++];
        else a[k++] = right[j++];
    }
    while (i < left.size()) a[k++] = left[i++];
    while (j < right.size()) a[k++] = right[j++];
}

void mergeSort(std::vector<int>& a, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(a, l, m);
        mergeSort(a, m + 1, r);
        merge(a, l, m, r);
    }
}

int partition(std::vector<int>& a, int low, int high) {
    int pivot = a[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (a[j] <= pivot) {
            ++i;
            std::swap(a[i], a[j]);
        }
    }
    std::swap(a[i + 1], a[high]);
    return i + 1;
}

void quickSort(std::vector<int>& a, int low, int high) {
    if (low < high) {
        int pi = partition(a, low, high);
        quickSort(a, low, pi - 1);
        quickSort(a, pi + 1, high);
    }
}

template<typename Func>
long long timeSort(std::vector<int> data, Func sorter) {
    auto start = std::chrono::high_resolution_clock::now();
    sorter(data);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int main() {
    const int N = 30000;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(1, 50000);
    
    std::vector<int> original(N);
    for (auto& x : original) x = dist(gen);
    
    std::cout << "Sorting Test" << std::endl;
    std::cout << "Array size: " << N << std::endl;
    
    std::cout << "Bubble sort: " << timeSort(original, [](std::vector<int>& v) { bubbleSort(v); }) << " ms" << std::endl;
    std::cout << "Insertion sort: " << timeSort(original, [](std::vector<int>& v) { insertionSort(v); }) << " ms" << std::endl;
    std::cout << "Merge sort: " << timeSort(original, [](std::vector<int>& v) { mergeSort(v, 0, v.size() - 1); }) << " ms" << std::endl;
    std::cout << "Quick sort: " << timeSort(original, [](std::vector<int>& v) { quickSort(v, 0, v.size() - 1); }) << " ms" << std::endl;
    std::cout << "std::sort: " << timeSort(original, [](std::vector<int>& v) { std::sort(v.begin(), v.end()); }) << " ms" << std::endl;
    
    return 0;
}