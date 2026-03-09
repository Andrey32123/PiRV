#include "teacher.hpp"
#include <iostream>

Teacher::Teacher() : Person(), discipline("") {}

Teacher::Teacher(std::string name) : Person(name), discipline("") {}

Teacher::Teacher(std::string name, std::string discipline) 
    : Person(name), discipline(discipline) {}

void Teacher::print() const {
    std::cout << "Teacher: " << name;
    if (!discipline.empty()) {
        std::cout << ", Discipline: " << discipline;
    }
    std::cout << std::endl;
}

std::string Teacher::getDiscipline() const {
    return discipline;
}

void Teacher::setDiscipline(std::string newDiscipline) {
    discipline = newDiscipline;
}