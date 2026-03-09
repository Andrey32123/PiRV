#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
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
void cing(std::vector<double>::iterator it,int j){
    double g;
    std::cin>>g;
    if ((0<=g)&& (g<=5)){
        *(it+j)=g*1.0;
    }else{
        std::cout<<"grade must be 0-5"<<'\n';
        std::cout<<"subj "<<j+1<<": ";
        cing(it,j);
    }
}

double averagest(std::vector<std::vector<double>>::iterator it,int i,int M){
    double m= double(M);
    double sum=0;
    for (int j=0;j<M;j++){
        sum+=(*(it+i))[j];
    }
    return sum/m;
}
double averagesb(std::vector<std::vector<double>>::iterator it,int i,int N){
    double n= double(N);
    double sum=0;
    for (int j=0;j<N;j++){
        sum+=(*(it+j))[i];
    }
    return sum/n;
}
#pragma pack(push,1)
struct Name{
        char signature;
        int version;
        int stdcnt;
    };
struct Grades{
    int subcnt;
    std::vector<std::vector<double>> grades;
};



#pragma pack(pop)




int main(){

    
    const int M=cinM();
    int N=cinN();
    
    //std::cout<<M<<' '<<N;
    std::vector<std::vector<double>> grades(N);
    std::vector<std::pair<int,double>> diary(N);
    std::vector<double> grades1(M);
    std::vector<double>::iterator g1 = grades1.begin();
    std::vector<std::vector<double>>::iterator it = grades.begin();
    for (int i=0;i<N;i++){
        std::cout<<"for student "<<i+1<<'\n';
        for (int j=0;j<M;j++){
            std::cout<<"subj "<<j+1<<": ";
            cing(g1,j);
        }
        grades[i]=(grades1);
        (diary[i])=std::make_pair (i+1,averagest(it,i,M));
    }

    
    diary.erase(std::remove_if(diary.begin(),diary.end(),[](std::pair<int,double> n){return n.second<2.5;}),diary.end());
    auto it2{std::remove_if(grades.begin(),grades.end(),[](std::vector<double> n){ double s=0;int sz=0;for (double ns:n){s+=ns;sz++;} return s/sz<2.5;})};
    grades.erase(it2,grades.end());
    N=grades.size();

    

    double max=0;
    
    for (int i=0;i<N;i++){
        double av=averagest(it,i,M);
        std::cout<<"average grade of student num"<<i+1<<": "<<av<<'\n';
        if (max<av){
            max=av;
        }
    }
    for (int i=0;i<M;i++){
        std::cout<<"average grade for subj num"<<i+1<<": "<<averagesb(it,i,M)<<'\n';
        
    }
    
    std::cout<<"max average st grade is "<<max<<'\n';
    std::cout<<'\n'<<"arr of grades:"<<'\n';
    for (int i=0;i<N*M;i++){
        std::cout<<(grades[i/M])[i%M]<<" ";
        if ((i+1)%M==0){
            std::cout<<'\n';
        }
    }
    
    std::sort(diary.begin(),diary.end(),[](const auto a,const auto b){if (a.second!=b.second){return a.second>b.second;}else{return a.first<b.first;}});
    for (int i=0;i<N;i++){
        std::cout<<"id with average grade: "<<diary[i].first<<" "<<diary[i].second<<'\n';
        
    }
    auto ntf =[diary]()->int{int s=0;for (std::pair<int,double> g:diary){if (g.second<3.0){s++;}} return s;};
    auto b =[diary]()->int{int s=0;for (std::pair<int,double> g:diary){if (g.second>=4.5){s++;}} return s;};
    
    std::cout<<"num of near to be fired: "<<ntf()<<'\n';
    std::cout<<"num of best: "<<b()<<'\n';

    Name NAME;
    Grades GRADES;
    NAME.signature='d';
    NAME.version=1;
    NAME.stdcnt=N;
    GRADES.subcnt=M;
    for (int i=0;i<N;i++){
        (GRADES.grades).push_back(grades[i]);  
    }
    //GRADES.grades=grades;

    std::ofstream ofs("data.dat", std::ios::out | std::ios::binary);
    if(ofs.is_open()) {
        ofs.write((char *)&NAME,sizeof(NAME));
        ofs.write((char *)&GRADES,sizeof(GRADES));
        
    }
    ofs.close();

    Name readName;
    Grades readGrades;
    int f;


    std::cout<<'\n'<<"Reading datawrong.dat..."<<'\n';
    std::ifstream ifswr("datawrong.dat", std::ios::in | std::ios::binary);
    if (ifswr.is_open()){
        ifswr.read((char *)&readName,sizeof(readName));
        ifswr.read((char *)&readGrades,sizeof(readGrades));
        
    }
    if (readName.signature=='d'){
        std::cout<<"correct signature"<<'\n';
        std::cout<<"signature: "<<readName.signature<<'\n'<<"version: "<<readName.version<<'\n'<<"num of students: "<<readName.stdcnt<<'\n';
        std::cout<<"num of subjects: "<<readGrades.subcnt<<'\n';
        std::cout<<'\n'<<"arr of grades:"<<'\n';
        for (int i=0;i<N*M;i++){
            std::cout<<((readGrades.grades)[i/M])[i%M]<<" ";
            if ((i+1)%M==0){
                std::cout<<'\n';
            }
        }
    }else{
        std::cout<<"wrong signature"<<'\n';
    }
    ifswr.close();







    std::cout<<'\n'<<"Reading data.dat..."<<'\n';
    std::ifstream ifs("data.dat", std::ios::in | std::ios::binary);
    

    if (ifs.is_open()){
        ifs.read((char *)&readName,sizeof(readName));
        ifs.read((char *)&readGrades,sizeof(readGrades));
        
    }
    
    ifs.close();
    if (readName.signature=='d'){
        std::cout<<"correct signature"<<'\n';
        std::cout<<"signature: "<<readName.signature<<'\n'<<"version: "<<readName.version<<'\n'<<"num of students: "<<readName.stdcnt<<'\n';
        std::cout<<"num of subjects: "<<readGrades.subcnt<<'\n';
        std::cout<<'\n'<<"arr of grades:"<<'\n';
        for (int i=0;i<N*M;i++){
            std::cout<<((readGrades.grades)[i/M])[i%M]<<" ";
            if ((i+1)%M==0){
                std::cout<<'\n';
            }
        }
    }else{
        std::cout<<"wrong signature"<<'\n';
    }



    std::cout<<"size of structure Name: "<<sizeof(readName)<<'\n';


    return 0;
}