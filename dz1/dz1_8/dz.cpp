#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <limits>
class RecordBook{
    public:
    int number;
    std::vector<double> grades;
};
class Person{
    protected:
    std::string name;
    
    public:
    virtual void print(){}
    virtual bool IsStudent(){
        return 1;
    }
};
class Teacher:public Person{
    public:
    Teacher() {
        name = "";
        
    }
    explicit Teacher(std::string nm)
    {
        name=nm;
    }
    void print() override{
        std::cout<<'\n'<<"Teacher "<<name<<'\n';
    }
    bool IsStudent() override{
        return false;
    }
};

class Student:public Person{
    private:
    
    std::string name;
    //std::vector<double> grades{};
    RecordBook rb;

    public:

    Student() {
        name = "";
    }
    explicit Student(std::string nm){
        name = nm;
    }
    bool IsStudent() override{
        return true;
    }
    double averageSb(){
        int n=rb.grades.size();
        double sum=0;
        for (int i=0;i<n;i++){
            sum+=rb.grades[i];
        }
        return sum/n;
    } 
    void addGrade(double grade){
        rb.grades.push_back(grade);
    }  
    void print() override{
        std::cout<<'\n'<<"Student "<<name<<":"<<'\n';
        std::cout<<"Grades:";
        for (double grade:rb.grades){
            std::cout<<" "<<grade;
        }
        std::cout<<'\n'<<"Average: "<<averageSb()<<'\n';
    }
};


int cinM(){
    int M;
    std::cout<<"num of subjects: ";
    std::cin>>M;
    if (M<=0){
        std::cout<<" num must be >0"<<'\n';
        cinM();
    }
    return M;
}
void createClass(std::vector<Person*>& myclass,int &Ns,int &Nt){
    
    std::string name="";
    int act=0;
    int count=0;
    while (name!="end"){
        std::cout<<"Creating class, '1' to add student, '2' to add teacher, '3' to end"<<'\n';
        if (!(std::cin >> act)) {
            std::cout << "Invalid input! Please enter a number."<<'\n';
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        switch(act){
            case 1:{
                Ns+=1;
                std::cout<<"Name of student number "<<Ns<<": ";
                std::cin>>name;
                while (name.length()<3){
                    std::cout<<"Name must be longer than 2,try again: ";
                    std::cin>>name;
                }
                Person* stptr = new Student(name);
                (myclass).push_back(stptr);
                name ="";
                break;}
            case 2:
                Nt+=1;{
                std::cout<<"Name of teacher: ";
                std::cin>>name;
                while (name.length()<3){
                    std::cout<<"Name must be longer than 2,try again: ";
                    std::cin>>name;
                }
                Person* stptr = new Teacher(name);
                (myclass).push_back(stptr);
                name ="";
                break;}
            case 3:{
                std::cout<<"Class created"<<'\n';
                name="end";
                break;}
            default:{
                std::cout<<"Undefined act, try again"<<'\n';
                break;}
        }
    }
}
void cinGrades(std::vector<Person*>& myclass, int Ns,int M){
    int c=0;
    for(int i=0;i<Ns;i++){
        while (!(*(myclass[i+c])).IsStudent()){
            c+=1;
        }
        Student* st=reinterpret_cast<Student*>(myclass[i+c]);
        std::cout<<"for Student number "<<i+1<<":"<<'\n';
        for (int j=0;j<M;j++){
            double gr=-1;
            while (gr<0 || gr>5){
                std::cout<<"Grade for subject number "<<j+1<<": ";
                std::cin>>gr;
                if (gr<0 || gr>5){
                    std::cout<<'\n'<<" Grade must be 0-5"<<'\n';
                }
            }
            (*st).addGrade(gr);
        }
    }
}
void getInfo(std::vector<Person*>& myclass,int N){
    std::cout<<'\n'<<"info about all students:"<<'\n';
    for (int i=0;i<N;i++){
        (*myclass[i]).print();
    }
}
int main(){
    int Ns=0,Nt=0,M;

    M=cinM();
    std::vector<Person*> myclass;
    std::vector<Person*>::iterator it=myclass.begin();
    createClass(myclass,Ns,Nt);
    cinGrades(myclass,Ns,M);
    
    getInfo(myclass,Ns+Nt);
    return 0;
}