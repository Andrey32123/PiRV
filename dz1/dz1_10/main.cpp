#include "functions.hpp"
#include "person.hpp"
#include "group.hpp"
#include <vector>

int main() {
    int Ns = 0, Nt = 0, M;

    M = cinM();
    std::vector<Person*> myclass;
    
    createClass(myclass, Ns, Nt);
    cinGrades(myclass, Ns, M);
    getInfo(myclass, Ns + Nt);

    Group mygroup = afterEGE(myclass);
    mygroup.getInfo();
    
    for (Person* p : myclass) {
        delete p;
    }
    
    return 0;
}