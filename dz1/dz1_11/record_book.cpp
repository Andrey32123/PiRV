#include "record_book.hpp"
#include <iostream>
#include <numeric>
#include <stdexcept>

RecordBook::RecordBook() : number(0) {}

RecordBook::RecordBook(int num) : number(num) {}

void RecordBook::addGrade(double grade) {
    if (grade < 0 || grade > 5) {
        std::cout<<"Grade must be between 0 and 5";
    }else{
    grades.push_back(grade);
    }
}

double RecordBook::Average() const {
    if (grades.empty()) return 0.0;
    
    double sum = 0.0;
    for (double grade : grades) {
        sum += grade;
    }
    return sum / grades.size();
}

void RecordBook::printGrades() const {
    std::cout << "Grades: ";
    for (double grade : grades) {
        std::cout << grade << " ";
    }
    std::cout << '\n';
}

int RecordBook::getNumber() const {
    return number;
}

const std::vector<double>& RecordBook::getGrades() const {
    return grades;
}

void RecordBook::setNumber(int num) {
    number = num;
}