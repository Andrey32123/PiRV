#include "student.hpp"
#include <iostream>

Student::Student() {
    name = "";
}

Student::Student(const std::string& nm) {
    name = nm;
}

bool Student::IsStudent() {
    return true;
}

double Student::averageSb() {
    return rb.averagegr();
}

void Student::addGrade(double grade) {
    rb.grades.push_back(grade);
}

void Student::print() {
    std::cout << '\n' << "Student " << name << ":" << '\n';
    std::cout << "Grades:";
    for (double grade : rb.grades) {
        std::cout << " " << grade;
    }
    std::cout << '\n' << "Average: " << averageSb() << '\n';
}