//
//  Process.hpp
//  CPU scheduling
//
//  Created by Hayam Hiba on 11/11/2022.
//

#ifndef Process_hpp
#define Process_hpp

#include <stdio.h>
#include <string>
#include <utility>

#endif /* Process_hpp */
class Process{
    
public:
    Process(int arrival,int service,char name){
        this->arrivalT = arrival;
        this->serviceT = service;
        this->name = name;
    }


    ~Process()=default;
    
public:
    int arrivalT;
    int serviceT;
    int finishT;
    int turnRT;
    double normT;
    char name;
    float level=0;
    int wait;
    int lastPush;

};
