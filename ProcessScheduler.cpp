//
//  ProcessScheduler.cpp
//  CPU scheduling
//
//  Created by Hayam Hiba on 11/11/2022.
//

#include "ProcessScheduler.hpp"

void ProcessScheduler::CalculateMean() {
    int sumOfTR = 0, sumOfNT = 0;
    for(auto x: this->processes){
        sumOfNT+= x->normT;
        sumOfTR+= x->turnRT;
    }
    this->meanTurnR = sumOfTR/(1.0*this->numOfProcess);
    this->meanNormT = sumOfNT/(1.0*this->numOfProcess);
}

void ProcessScheduler::Prepare2DMatrix() {

}
//1. FCFS (First Come First Serve)
//2. RR (Round Robin)
//3. SPN (Shortest Process Next)
//4. SRT (Shortest Remaining Time)
//5. HRRN (Highest Response Ratio Next)
//6. FB-1, (Feedback where all queues have q=1)
//7. FB-2i, (Feedback where q= 2i)
//8. Aging
void ProcessScheduler::StartScheduler() {
    switch(this->policy){
        case 1:
            FCFSSchedule();
            break;
        case 2:
            RRSchedule(this->quantum);
            break;
        case 3:
            SPN();
            break;
        case 4:
            SRT();
            break;
        case 5:
            HRRN();
            break;
        case 6:
            FB(this->quantum);
            exit;
        case 7:
            FB2();
            break;
        case 8:
            Aging();
            break;
        default:
            printf("Error parsing input file");
            exit(1);
    }
}

void ProcessScheduler::PrintSchedule(std::string d) {
    int numOfRows,numOfCols;
    if(d == "trace"){
        numOfCols = this->lastInst+1;
        numOfRows = this->numOfProcess;
    }
    else{
        numOfCols = this->numOfProcess;
        numOfRows = 6;
    }
    for(int i =0 ;i<numOfRows;i++){
        for(int j=0;j<numOfCols;j++){
            printf("%c|",this->matrixOfSchedule[i][j]);
        }
        printf("\n");
    }
}

void ProcessScheduler::FCFSSchedule() {

}

void ProcessScheduler::RRSchedule(int q) {

}

void ProcessScheduler::FB(int q) {

}

void ProcessScheduler::FB2() {

}

void ProcessScheduler::Aging() {

}

void ProcessScheduler::SPN() {

}

void ProcessScheduler::SRT() {

}

void ProcessScheduler::HRRN() {

}

