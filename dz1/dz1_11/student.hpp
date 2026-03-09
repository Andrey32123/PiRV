#pragma once

#include "person.hpp"
#include "record_book.hpp"
#include <string>

class Student : public Person {
private:
    RecordBook recordBook;
    
public:
    Student();
    explicit Student(const std::string& name);
    Student(const std::string& name, int recordBookNumber);
    
    void print() const override;
    double Average() const;
    void addGrade(double grade);
    void printGrades() const;
    int getRecordBookNumber() const;
    void setRecordBookNumber(int num);
    RecordBook& getRecordBook();
};