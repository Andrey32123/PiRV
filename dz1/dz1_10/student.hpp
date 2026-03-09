#pragma once
#include <iostream>
#include "person.hpp"
#include "record_book.hpp"
#include <string>

class Student : public Person {
private:
    std::string name;
    RecordBook rb;

public:
    Student();
    explicit Student(const std::string& nm);
    bool IsStudent() override;
    double averageSb();
    void addGrade(double grade);
    void print() override;
    

    inline void printName() const {
        std::cout << "Student name: " << name << std::endl;
    }
};