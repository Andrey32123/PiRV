#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>

class Student{
    private:

    std::string name;
    std::vector<double> grades{};


    public:

    Student() : name("") {}
    explicit Student(std::string nm):name(nm){}

    double averageSb(){
        int n=grades.size();
        double sum=0;
        for (int i=0;i<n;i++){
            sum+=grades[i];
        }
        return sum/n;
    } 
    void addGrade(double grade){
        grades.push_back(grade);
    }  
    void info(){
        std::cout<<"Name: "<<name<<'\n';
        std::cout<<"Grades:";
        for (double grade:grades){
            std::cout<<" "<<grade;
        }
        std::cout<<'\n'<<"Average: "<<averageSb()<<'\n';
    }
};


int cinN(){
    int N;
    std::cout<<"num of students: ";
    std::cin>>N;
    if (N<=0){
        std::cout<<" num must be >0"<<'\n';
        cinN();
    }
    return N;
}
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
void createClass(std::vector<Student>& students,int N){
    for (int i=0;i<N;i++){
        std::string name;
        std::cout<<"Name of student number "<<i+1<<": ";
        std::cin>>name;
        while (name.length()<3){
            std::cout<<"Name must be longer than 2,try again: ";
        std::cin>>name;
        }
        students.push_back(Student(name));
    }
}
void cinGrades(std::vector<Student>& students, int N,int M){
    
    for(int i=0;i<N;i++){
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
            students[i].addGrade(gr);
        }
    }
}
void getInfo(std::vector<Student>& students,int N){
    std::cout<<"info about all students:"<<'\n';
    for (int i=0;i<N;i++){
        students[i].info();
    }
}
int main(){
    int N,M;
    N=cinN();
    M=cinM();
    std::vector<Student> students;
    std::vector<Student>::iterator it=students.begin();
    createClass(students,N);
    cinGrades(students,N,M);
    getInfo(students,N);
    return 0;
}