#include "group.hpp"
#include "student.hpp"
#include <algorithm>
#include <iostream>

Group::Group(const std::vector<Student*>& gr, const std::string& nm) {
    name = nm;
    group = gr;
}

void Group::Add(Student* st) {
    group.push_back(st);
}

void Group::Delete(int i) {
    auto it2 = std::remove_if(group.begin(), group.end(), 
        [](Student* n) { return n->averageSb() < 2.5; });
    group.erase(it2, group.end());
}

double Group::AvGrade() {
    double sum = 0;
    double N = 0;
    for (Student* s : group) {
        N++;
        sum += s->averageSb();
    }
    return N>0?sum/N:0.0;
}

void Group::getInfo() {
    std::cout << "Info about group " << name << ":\n";
    std::cout << "Average grade of group is " << AvGrade() << '\n';
    std::cout << "Students:\n";
    for (Student* s : group) {
        s->print();
    }
}