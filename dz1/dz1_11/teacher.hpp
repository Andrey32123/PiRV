#pragma once

#include "person.hpp"
#include <string>

class Teacher : public Person {
private:
    std::string discipline;
    
public:
    Teacher();
    explicit Teacher(std::string name);
    Teacher(std::string name, std::string discipline);
    
    void print() const override;
    std::string getDiscipline() const;
    void setDiscipline(std::string newDiscipline);
};