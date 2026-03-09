#pragma once

#include <vector>

class RecordBook {
public:
    int number;
    std::vector<double> grades;
    double averagegr();
};