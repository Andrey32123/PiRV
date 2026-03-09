#pragma once

#include <vector>

class RecordBook {
private:
    int number;
    std::vector<double> grades;
    
public:
    RecordBook();
    explicit RecordBook(int num);
    
    void addGrade(double grade);
    double Average() const;
    void printGrades() const;
    int getNumber() const;
    const std::vector<double>& getGrades() const;
    void setNumber(int num);
};