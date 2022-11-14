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
            break;
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
//TODO: make a print for trace and a print for stats

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

    for (auto x: this->processes) {
        printf("%c %d %d %d %d %.2f\n", x->name, x->arrivalT, x->serviceT, x->finishT, x->turnRT, x->normT);
    }
    printf(" meanTurnR= %f meanNormT=%f\n", this->meanTurnR, this->meanNormT);

    //make sure processes are sorted by arrival time

}

void ProcessScheduler::RRSchedule(int q) {
    //TODO: meanNormT=2.00 != 2.54 recheck CalculateMean()
    std::queue<Process *> ready_p;
    std::vector<char> result;
    int timer = this->processes.at(0)->arrivalT, qtm = q, process_idx = 1;
    ready_p.push(this->processes.at(0));
    Process *x;
    int arr[this->numOfProcess];
    for (int p = 0; p < this->numOfProcess; p++)
        arr[p] = this->processes.at(p)->serviceT;

    while (!ready_p.empty() && timer < this->lastInst) {
        x = ready_p.front();
        ready_p.pop();
        while (qtm > 0 && x->serviceT > 0) {
            result.emplace_back(x->name);
            qtm--;
            timer++;
            x->serviceT--;

            for (int j = process_idx; j < this->numOfProcess; j++) {
                if (this->processes.at(j)->arrivalT > timer) {
                    process_idx = j;
                    break;
                }
                if (this->processes.at(j)->arrivalT == timer) {
                    ready_p.push(this->processes.at(j));
                }
            }

        }
        if (x->serviceT > 0)
            ready_p.push(x);
        else
            x->finishT = timer;
        qtm = q;
    }

    for (int p = 0; p < this->numOfProcess; p++)
        this->processes.at(p)->serviceT = arr[p];

    for (auto p: this->processes) {
        for (int i = 0; i < p->arrivalT; i++) {
            printf(" |");
        }

        for (int i = p->arrivalT; i < p->finishT; i++) {
            if (result.at(i) != p->name) {
                printf(".|");
            } else {
                printf("*|");
            }
        }

        for (int i = p->finishT; i < this->lastInst; i++)
            printf(" |");

        p->turnRT = p->finishT - p->arrivalT;
        p->normT = (p->turnRT * 1.0) / p->serviceT;
        printf("\n");
    }
    CalculateMean();
    for (auto x: this->processes) {
        printf("%c %d %d %d %d %.2f\n", x->name, x->arrivalT, x->serviceT, x->finishT, x->turnRT, x->normT);
    }
    printf(" meanTurnR= %f meanNormT=%f\n", this->meanTurnR, this->meanNormT);

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

