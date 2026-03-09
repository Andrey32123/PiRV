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
class Group{
    private:
    std::vector<Student*> group;
    std::string name;
    public:
    explicit Group(std::vector<Student*> gr,std::string nm){
        name = nm;
        group = gr;
    }
    void Add(Student* st){
        group.push_back(st);
    }
    void Delete(int i){
        auto it2{std::remove_if(group.begin(),group.end(),[](Student* n){return n->averageSb()<2.5;})};
        group.erase(it2,group.end());
    }
    double AvGrade(){
        double sum=0;
        double N=0;
        for (Student* s:group){
            N++;
            sum+=s->averageSb();
        }
        return sum/N;
    }
    void getInfo(){
        std::cout<<"Info about group "<<name<<":\n";
        std::cout<<"Average grade of group is "<<AvGrade()<<'\n';
        std::cout<<"Students:\n";
        for (Student* s:group){
            s->print();
        }
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

Group afterEGE(std::vector<Person*> target){
    std::cout<<"Exams finished, creating the group...\n";
    std::string name="";
    while (name.length()<3){
        std::cout<<"Enter the name of the group(len>=3): ";
        std::cin>>name;
    }
    std::vector<Student*> group;
    Group gr(group,name);
    std::vector<int> undeads;
    int i=0;
    for (Person* t:target){
        if (t->IsStudent()){
            Student* st = reinterpret_cast<Student*>(t);
            gr.Add(st);
            i++;
            if (st->averageSb()<2.5){
                undeads.push_back(i);
            }
        }
        
    }
    for (int j:undeads){
    gr.Delete(j);
    }
    std::cout<<"Creating completed\n";
    return gr;
}

int main(){

    int Ns=0,Nt=0,M;

    M=cinM();
    std::vector<Person*> myclass;
    std::vector<Person*>::iterator it=myclass.begin();
    createClass(myclass,Ns,Nt);
    cinGrades(myclass,Ns,M);
    getInfo(myclass,Ns+Nt);

    Group mygroup = afterEGE(myclass);
    mygroup.getInfo();
    
    return 0;
}