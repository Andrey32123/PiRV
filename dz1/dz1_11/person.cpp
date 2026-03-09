#include "person.hpp"

Person::Person() : name("") {}

Person::Person(const std::string& name) : name(name) {}

std::string Person::getName() const {
    return name;
}
void Person::print() const{}
void Person::setName(const std::string& newName) {
    name = newName;
}