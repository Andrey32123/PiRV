#pragma once

#include <vector>
#include <string>
#include <memory>

class Student;

class Group {
private:
    std::string name;
    std::vector<Student*> students;
    
public:
    Group();
    explicit Group(std::string name);

    
    void addStudent(Student* student);
    bool removeStudent(std::string& studentName);
    void printAll();
    double Average();
    void sort();
    std::vector<Student*> filter(double grade);
    int getSize();
    std::string getName();
    Student* getStudent(int index);
    
};