#pragma once

#include<iostream>

class Person {
protected:
    std::string name;
    
public:
    Person();
    explicit Person(const std::string& name);
    
    
    virtual void print() const;
    std::string getName() const;
    void setName(const std::string& newName);
};