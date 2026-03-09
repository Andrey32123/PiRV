#include "record_book.hpp"
#include<iostream>

double RecordBook::averagegr(){
    int c=0;
    double sum=0;
    for (double gr:grades){
        sum+=gr;
        c+=1;
    }
    return c!=0?sum/c:0.0;
}