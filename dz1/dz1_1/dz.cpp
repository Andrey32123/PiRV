double srar(double *ptr,int N){
    double srar=0;
    for (int i=0;i<N;i++){
        srar+=*(ptr+i);
    }
    return srar/N;
}
double mx(double *ptr,int N){
    double mx=-1;
    for (int i=0;i<N;i++){
        if(*(ptr+i)>mx){
            mx=*(ptr+i);
        }
    }
    return mx;
}
double mn(double *ptr,int N){
    double mn=6;
    for (int i=0;i<N;i++){
        if(*(ptr+i)<mn){
            mn=*(ptr+i);
        }
    }
    return mn;
}
int better_count(double *ptr,int N,double x){
    int cnt=0;
    for (int i=0;i<N;i++){
        if(*(ptr+i)>x){
            cnt++;
        }
    }
    return cnt;
}



#include <iostream>
int main(){
    int N;
    std::cout<<"number of students: ";
    std::cin>>N;
    if (N<=0){
        while (N<=0){
            std::cout<<"number must be >0"<<'\n'<<"number of students: ";
            std::cin>>N;
        }
    }
    std::cout<<'\n';
    double* grades = new double[N];
    
    for(int i=0;i<N;i++){
        
        double gr=-1;
        while (gr<0 || gr>5){
            std::cout<<"enter grade for the student number "<<i+1<<": ";
            std::cin>>gr;
            if (gr<0 || gr>5){
                std::cout<<"grade must be 0-5"<<'\n';
            }
        }
        grades[i]=gr;
    }
    std::cout<<"grades: ";
    for (int i=0;i<N;i++){
        std::cout<<grades[i]<<' ';
    }
    std::cout<<'\n';
    double *ptr=grades;
    std::cout<<"the average grate is "<<srar(ptr,N)<<'\n';
    std::cout<<"the max grate is "<<mx(ptr,N)<<'\n';
    std::cout<<"the min grate is "<<mn(ptr,N)<<'\n';
    std::cout<<"number of students with a grade higher than 3: "<<better_count(ptr,N,3)<<'\n';
    delete[] grades;
    return 0;
}