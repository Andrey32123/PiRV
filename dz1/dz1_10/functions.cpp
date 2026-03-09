#include "functions.hpp"
#include "person.hpp"
#include "student.hpp"
#include "teacher.hpp"
#include "group.hpp"
#include <iostream>
#include <limits>
#include <algorithm>

int cinM() {
    int M;
    std::cout << "num of subjects: ";
    std::cin >> M;
    if (M <= 0) {
        std::cout << " num must be >0" << '\n';
        return cinM();
    }
    return M;
}

void createClass(std::vector<Person*>& myclass, int& Ns, int& Nt) {
    std::string name = "";
    int act = 0;
    
    while (name != "end") {
        std::cout << "Creating class, '1' to add student, '2' to add teacher, '3' to end" << '\n';
        
        if (!(std::cin >> act)) {
            std::cout << "Invalid input! Please enter a number." << '\n';
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        switch(act) {
            case 1: {
                Ns += 1;
                std::cout << "Name of student number " << Ns << ": ";
                std::cin >> name;
                while (name.length() < 3) {
                    std::cout << "Name must be longer than 2, try again: ";
                    std::cin >> name;
                }
                Person* stptr = new Student(name);
                myclass.push_back(stptr);
                name = "";
                break;
            }
            case 2: {
                Nt += 1;
                std::cout << "Name of teacher: ";
                std::cin >> name;
                while (name.length() < 3) {
                    std::cout << "Name must be longer than 2, try again: ";
                    std::cin >> name;
                }
                Person* stptr = new Teacher(name);
                myclass.push_back(stptr);
                name = "";
                break;
            }
            case 3: {
                std::cout << "Class created" << '\n';
                name = "end";
                break;
            }
            default: {
                std::cout << "Undefined act, try again" << '\n';
                break;
            }
        }
    }
}

void cinGrades(std::vector<Person*>& myclass, int Ns, int M) {
    int c = 0;
    for(int i = 0; i < Ns; i++) {
        while (!(*(myclass[i + c])).IsStudent()) {
            c += 1;
        }
        Student* st = reinterpret_cast<Student*>(myclass[i + c]);
        std::cout << "for Student number " << i + 1 << ":" << '\n';
        
        for (int j = 0; j < M; j++) {
            double gr = -1;
            while (gr < 0 || gr > 5) {
                std::cout << "Grade for subject number " << j + 1 << ": ";
                std::cin >> gr;
                if (gr < 0 || gr > 5) {
                    std::cout << '\n' << " Grade must be 0-5" << '\n';
                }
            }
            st->addGrade(gr);
        }
    }
}

void getInfo(std::vector<Person*>& myclass, int N) {
    std::cout << '\n' << "info about all students:" << '\n';
    for (int i = 0; i < N; i++) {
        myclass[i]->print();
    }
}

Group afterEGE(std::vector<Person*> target) {
    std::cout << "Exams finished, creating the group...\n";
    std::string name = "";
    
    while (name.length() < 3) {
        std::cout << "Enter the name of the group(len>=3): ";
        std::cin >> name;
    }
    
    std::vector<Student*> group;
    Group gr(group, name);
    std::vector<int> undeads;
    int i = 0;
    
    for (Person* t : target) {
        if (t->IsStudent()) {
            Student* st = reinterpret_cast<Student*>(t);
            gr.Add(st);
            i++;
            if (st->averageSb() < 2.5) {
                undeads.push_back(i);
            }
        }
    }
    
    for (int j : undeads) {
        gr.Delete(j);
    }
    
    std::cout << "Creating completed\n";
    return gr;
}