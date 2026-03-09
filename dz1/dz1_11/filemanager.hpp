#pragma once

#include <string>
#include <memory>

class Group;

class FileManager {
public:
    static bool saveGroup(Group& group, std::string filename);
    static Group* loadGroup(std::string filename);
    static bool validateFile(std::string filename);
    
private:
    static const int sgn = 0x47525000; // "GRP"
};