#include "filemanager.hpp"
#include "group.hpp"
#include "student.hpp"
#include <fstream>
#include <iostream>
#include <vector>
const int FileManager::sgn;
bool FileManager::saveGroup(Group& group, std::string filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        
        file.write(reinterpret_cast<const char*>(sgn), sizeof(sgn));
       
        int nameLength = group.getName().size();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(group.getName().c_str(), nameLength);
        
        int studentCount = group.getSize();
        file.write(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
        
        
        for (int i = 0; i < studentCount; ++i) {
            Student* student = group.getStudent(i);
            
            int namelen = student->getName().size();
            file.write(reinterpret_cast<char*>(&namelen), sizeof(namelen));
            file.write(student->getName().c_str(), namelen);
            
            int recordNumber = student->getRecordBookNumber();
            file.write(reinterpret_cast<char*>(&recordNumber), sizeof(recordNumber));
            
            const auto& grades = student->getRecordBook().getGrades();
            int gradesCount = grades.size();
            file.write(reinterpret_cast<char*>(&gradesCount), sizeof(gradesCount));
            
            for (double grade : grades) {
                file.write(reinterpret_cast<char*>(&grade), sizeof(grade));
            }
        }
        
        file.close();
        return true;
    }
    return false;
}

Group* FileManager::loadGroup(std::string filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()){

        int signature;
        file.read(reinterpret_cast<char*>(&signature), sizeof(signature));
        if (signature != sgn) {
            std::cerr << "Error: wrong file " << std::endl;
            file.close();
            return nullptr;
        }
        
        int nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        std::string groupName(nameLength, ' ');
        file.read(&groupName[0], nameLength);
        
        auto group = Group(groupName);
        
        int studentCount;
        file.read(reinterpret_cast<char*>(&studentCount), sizeof(studentCount));
        
 
        for (int i = 0; i < studentCount; ++i) {
   
            int studentNameLength;
            file.read(reinterpret_cast<char*>(&studentNameLength), sizeof(studentNameLength));
            std::string studentName(studentNameLength, ' ');
            file.read(&studentName[0], studentNameLength);
            
            int recordNumber;
            file.read(reinterpret_cast<char*>(&recordNumber), sizeof(recordNumber));
            
            auto student = new Student(studentName, recordNumber);
            
            int gradesCount;
            file.read(reinterpret_cast<char*>(&gradesCount), sizeof(gradesCount));
            
            for (int j = 0; j < gradesCount; ++j) {
                double grade;
                file.read(reinterpret_cast<char*>(&grade), sizeof(grade));
                student->addGrade(grade);
            }
            
            group.addStudent(student);
        }
        
        file.close();
        
    }
    return nullptr;
}

bool FileManager::validateFile(std::string filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    int signature;
    file.read(reinterpret_cast<char*>(&signature), sizeof(signature));
    file.close();
    
    return signature == sgn;
}