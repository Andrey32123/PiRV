#pragma once

#include "person.hpp"
#include <string>

class Teacher : public Person {
public:
    Teacher();
    explicit Teacher(const std::string& nm);
    void print() override;
    bool IsStudent() override;
};