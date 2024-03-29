//
//  ProcessScheduler.hpp
//  CPU scheduling
//
//  Created by Hayam Hiba on 11/11/2022.
//

//1. FCFS (First Come First Serve)
//2. RR (Round Robin)
//3. SPN (Shortest Process Next)
//4. SRT (Shortest Remaining Time)
//5. HRRN (Highest Response Ratio Next)
//6. FB-1, (Feedback where all queues have q=1)
//7. FB-2i, (Feedback where q= 2i)
//8. Aging

#ifndef ProcessScheduler_hpp
#define ProcessScheduler_hpp

#include <stdio.h>
#include <utility>
#include <vector>
#include <iostream>
#include "Process.hpp"

#endif /* ProcessScheduler_hpp */
class ProcessScheduler{
    
public:
    ProcessScheduler(int policy,std::string display,int lastInstant,int numOfProcess,int quantum){
        this->policy = policy;
        this->display = std::move(display);
        this->lastInst = lastInstant;
        this->numOfProcess = numOfProcess;
        this->quantum = quantum;
        this->traceDisplay = (char*) malloc(sizeof(char)*numOfProcess*lastInstant);
        this->results.clear();
        //matrixOfSchedule = (char**)malloc(sizeof(char*) * numOfProcess);
    }
    
    ~ProcessScheduler()=default;
    
    
    void CalculateMean();
    void StartScheduler();
    
    void FCFSSchedule();
    void RRSchedule(int quantum);
    void SPN();
    void SRT();
    void HRRN();
    void FB();
    void FB2();
    void Aging(int q);
    void PrintTraceResults();
    void PrintStatsResults();

public:
    int policy;
    std::string display;
    int lastInst;
    int quantum;
    int numOfProcess;
    std::vector<Process*> processes;
    char** matrixOfSchedule;
    float meanTurnR, meanNormT;
    char* traceDisplay;
    char* statsDisplay;
    std::vector<char> results;
};
