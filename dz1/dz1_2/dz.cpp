#include <iostream>
#include <vector>

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


int main(){
    const int M=cinM();
    const int N=cinN();
    std::cout<<M<<' '<<N;
    std::vector<std::vector<double>> grades(N);
    
    std::vector<double> grades1(M);
    std::vector<double>::iterator g1 = grades1.begin();
    for (int i=0;i<N;i++){
        std::cout<<"for student "<<i+1<<'\n';
        for (int j=0;j<M;j++){
            std::cout<<"subj "<<j+1<<": ";
            cing(g1,j);
        }
        grades[i]=(grades1);
    }
    double max=0;
    std::vector<std::vector<double>>::iterator it = grades.begin();
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
    return 0;
}