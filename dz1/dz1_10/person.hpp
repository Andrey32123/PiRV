#pragma once

#include <string>

class Person {
protected:
    std::string name;
    
public:
    virtual ~Person() = default;
    virtual void print();
    virtual bool IsStudent();
};