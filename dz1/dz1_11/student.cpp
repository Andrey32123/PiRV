#include "student.hpp"
#include <iostream>

Student::Student() : Person() {}

Student::Student(const std::string& name) : Person(name) {}

Student::Student(const std::string& name, int recordBookNumber) 
    : Person(name), recordBook(recordBookNumber) {}

void Student::print() const {
    std::cout << "Student: " << name << std::endl;
    std::cout << "Record Book #" << recordBook.getNumber() << std::endl;
    recordBook.printGrades();
    std::cout << "Average: " << Average() << std::endl;
}

double Student::Average() const {
    return recordBook.Average();
}

void Student::addGrade(double grade) {
    recordBook.addGrade(grade);
}

void Student::printGrades() const {
    recordBook.printGrades();
}

int Student::getRecordBookNumber() const {
    return recordBook.getNumber();
}

void Student::setRecordBookNumber(int num) {
    recordBook.setNumber(num);
}

RecordBook& Student::getRecordBook() {
    return recordBook;
}