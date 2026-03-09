#include "group.hpp"
#include "student.hpp"
#include <algorithm>
#include <iostream>

Group::Group() : name("") {}

Group::Group(std::string name) : name(name) {}


void Group::addStudent(Student* student) {
    if (student) {
        students.push_back(student);
    }
}

bool Group::removeStudent(std::string& studentName) {
    auto it = std::remove_if(students.begin(), students.end(),
        [&studentName](Student* s) { return s->getName() == studentName; });
    
    if (it != students.end()) {
        students.erase(it,students.end());
        return true;
    }
    return false;
}

void Group::printAll() {
    std::cout << "Group: " << name << '\n';
    std::cout << "Students:" << '\n';
    for (int i = 0; i < students.size(); ++i) {
        std::cout << i + 1 << ". ";
        students[i]->print();
        std::cout << '\n';
    }
}

double Group::Average() {
    if (students.empty()) return 0.0;
    
    double sum = 0.0;
    for (Student* s : students) {
        sum += s->Average();
    }
    return sum / students.size();
}

void Group::sort() {
    std::sort(students.begin(), students.end(),
        [](Student* a, Student* b) {
            return a->Average() > b->Average();
        });
}

std::vector<Student*> Group::filter(double grade) {
    std::vector<Student*> result;
    for (Student* s : students) {
        if (s->Average() >= grade) {
            result.push_back(s);
        }
    }
    return result;
}

int Group::getSize(){
    return students.size();
}

std::string Group::getName() {
    return name;
}

Student* Group::getStudent(int index) {
    if (index >= 0 && index < students.size()) {
        return students[index];
    }
    return nullptr;
}