#pragma once

#include <vector>


class Person;
class Group;

int cinM();
void createClass(std::vector<Person*>& myclass, int& Ns, int& Nt);
void cinGrades(std::vector<Person*>& myclass, int Ns, int M);
void getInfo(std::vector<Person*>& myclass, int N);
Group afterEGE(std::vector<Person*> target);