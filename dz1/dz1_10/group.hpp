#pragma once

#include <vector>
#include <string>


class Student;

class Group {
private:
    std::vector<Student*> group;
    std::string name;
    
public:
    explicit Group(const std::vector<Student*>& gr, const std::string& nm);
    void Add(Student* st);
    void Delete(int i);
    double AvGrade();
    void getInfo();
};