//
//  ProcessScheduler.cpp
//  CPU scheduling
//
//  Created by Hayam Hiba on 11/11/2022.
//

#include "ProcessScheduler.hpp"
#include <queue>

//TODO: precisin should be same as in test file

void ProcessScheduler::CalculateMean() {
    int sumOfTR = 0, sumOfNT = 0;
    for (auto x: this->processes) {
        sumOfNT += x->normT;
        sumOfTR += x->turnRT;
    }
    this->meanTurnR = sumOfTR / (1.0 * this->numOfProcess);
    this->meanNormT = sumOfNT / (1.0 * this->numOfProcess);
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
    switch (this->policy) {
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
//    int numOfRows, numOfCols;
//    if (d == "trace") {
//        numOfCols = this->lastInst + 1;
//        numOfRows = this->numOfProcess;
//        this->traceDisplay = (char *) malloc(sizeof(char) * numOfCols * numOfRows);
//    } else {
//        numOfCols = this->numOfProcess;
//        numOfRows = 6;
//        this->traceDisplay = (char *) malloc(sizeof(char) * numOfCols * numOfRows);
//    }
    printf("---------------------------------------\n");
    for (int i = 0; i < this->numOfProcess; i++) {
        for (int j = 0; j < this->lastInst; j++) {
            printf("%c|", *(this->traceDisplay + i * this->numOfProcess + j));
        }
        printf("\n");
    }
}

void ProcessScheduler::FCFSSchedule() {
    int wait = 0;
    Process *x;
    for (int j = 0; j < this->numOfProcess; j++) {
        x = this->processes.at(j);
        for (int i = 0; i < x->arrivalT; i++) {
            *(this->traceDisplay + j * this->numOfProcess + i) = '*';
            printf(" |");
        }

        for (int i = x->arrivalT; i < wait; i++) {
            *(this->traceDisplay + j * this->numOfProcess + i) = '.';
            printf(".|");
        }

        for (int i = 0; i < x->serviceT; i++) {
            *(this->traceDisplay + j * this->numOfProcess + i) = '*';
            printf("*|");
        }

        for (int i = wait + x->serviceT; i < this->lastInst; i++) {
            *(this->traceDisplay + j * this->numOfProcess + i) = ' ';
            printf(" |");
        }
        x->finishT = wait + x->serviceT;
        x->turnRT = x->finishT - x->arrivalT;
        x->normT = (x->turnRT * 1.0) / x->serviceT;
        CalculateMean();
        wait = x->serviceT + wait;
        printf("\n");
    }
//
//    printf("  ");
//    for (int j = 0; j < this->lastInst; j++){
//        printf("%d ",j%(this->lastInst/2));
//    }
//
//    printf("\n");
//    for (int i = 0; i < this->numOfProcess; i++) {
//        Process *x = this->processes.at(i);
//        printf("%s ",x->name.c_str());
//        for (int j = 0; j < this->lastInst; j++) {
//            if (i == 0  && x->serviceT > 0) {
//                *(this->traceDisplay + i * this->numOfProcess + j) = '*';
//               // printf("*|");
//                x->serviceT--;
//            } else {
//                if (x->arrivalT <= j && x->serviceT > 0) {
//                    char c = *(this->traceDisplay + ((i - 1) * this->numOfProcess) + j);
//                    if (c == '*' || c == '.') {
//                        *(this->traceDisplay + i * this->numOfProcess + j) = '.';
////                        printf(".|");
//                    } if(c == ' ') {
//                        *(this->traceDisplay + i * this->numOfProcess + j) = '*';
//                      //  printf("*|");
//                        x->serviceT--;
//                    }
//                } else {
//                    *(this->traceDisplay + i * this->numOfProcess + j) = ' ';
//                   // printf(" |");
//                }
//            }
//        }
//        printf("\n");
//    }

    //make sure processes are sorted by arrival time


}

void ProcessScheduler::RRSchedule(int q) {
    std::queue<Process*> pq;
    int tq = q,j=1;
    pq.push(this->processes.at(0));
    for(int i =0 ;i<this->lastInst;i++){
        while(!pq.empty()){
            Process* x= pq.front();
            pq.pop();
            while(tq>0){
                printf("*|");
                tq--;
            }
            for(;j<this->numOfProcess;){

            }
        }
    }
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

