#include <iostream>
#include <vector>
#include <memory>
#include "student.hpp"
#include "teacher.hpp"
#include "group.hpp"
#include "filemanager.hpp"

int main() {
    std::vector<Person*> people;

    std::cout << "Creating students" << std::endl;
    
    Student* student1 = new Student("gleb", 1001);
    student1->addGrade(4.5);
    student1->addGrade(5.0);
    student1->addGrade(4.0);
    student1->addGrade(4.5);
    people.push_back(student1);
    
    Student* student2 = new Student("andrey", 1002);
    student2->addGrade(3.5);
    student2->addGrade(4.0);
    student2->addGrade(3.0);
    student2->addGrade(3.5);
    people.push_back(student2);
    
    Student* student3 = new Student("valera", 1003);
    student3->addGrade(5.0);
    student3->addGrade(5.0);
    student3->addGrade(4.5);
    student3->addGrade(5.0);
    people.push_back(student3);
    
    Student* student4 = new Student("Dmitry Skryabikov", 1004);
    student4->addGrade(2.5);
    student4->addGrade(3.0);
    student4->addGrade(2.0);
    student4->addGrade(2.5);
    people.push_back(student4);
    

    Teacher* teacher = new Teacher("Archimed", "Math");
    people.push_back(teacher);
    
    std::cout << "\nCreating group" << std::endl;
    Group group("IU-2026");
    
    group.addStudent(student1);
    group.addStudent(student2);
    group.addStudent(student3);
    group.addStudent(student4);
    

    std::cout << "\nGroup information" << std::endl;
    group.printAll();
    std::cout << "Group average: " << group.Average() << std::endl;
    

    std::cout << "\nSorted by average" << std::endl;
    group.sort();
    group.printAll();
    
    
    std::cout << "\nStudents with average >= 4.0" << std::endl;
    auto excellentStudents = group.filter(4.0);
    for (size_t i = 0; i < excellentStudents.size(); ++i) {
        std::cout << i + 1 << ". " << excellentStudents[i]->getName() 
                  << " avgrade: " << excellentStudents[i]->Average() << '\n';
    }
    
    std::cout << "\nSaving group to file" << std::endl;
    std::string filename = "group.bin";
    if (FileManager::saveGroup(group, filename)) {
        std::cout << "Group saved successfully to " << filename << std::endl;
    } else {
        std::cout << "Failed to save group" << std::endl;
    }
    
    std::cout << "\nValidating file" << std::endl;
    if (FileManager::validateFile(filename)) {
        std::cout << "File is valid" << std::endl;
    } else {
        std::cout << "File is invalid" << std::endl;
    }
    
    std::cout << "\nLoading group from file" << std::endl;
    auto loadedGroup = FileManager::loadGroup(filename);
    if (loadedGroup) {
        std::cout << "Group loaded successfully:" << std::endl;
        loadedGroup->printAll();
    } else {
        std::cout << "Failed to load group" << std::endl;
    }
    
    std::cout << "\nRemoving student Dmitry Skryabikov" << std::endl;
    std::string DS = "Dmitry Skryabikov";
    if (group.removeStudent(DS)) {
        std::cout << "Student removed successfully" << std::endl;
        group.printAll();
    }
    
    std::cout << "\nAll people" << std::endl;
    for (auto& person : people) {
        person->print();
        std::cout << std::endl;
    }


    std::cout << "\nCleaning up" << std::endl;
    for (auto person : people) {
        delete person;
    }
    people.clear();
    
    
    return 0;
}