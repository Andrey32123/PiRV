#include "teacher.hpp"
#include <iostream>

Teacher::Teacher() {
    name = "";
}

Teacher::Teacher(const std::string& nm) {
    name = nm;
}

void Teacher::print() {
    std::cout << '\n' << "Teacher " << name << '\n';
}

bool Teacher::IsStudent() {
    return false;
}