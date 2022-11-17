//
//  ProcessScheduler.cpp
//  CPU scheduling
//
//  Created by Hayam Hiba on 11/11/2022.
//

#include "ProcessScheduler.hpp"
#include <queue>
#include <set>
#include <cmath>

using namespace std;

struct LessThanByAge {
    bool operator()(const Process *lhs, const Process *rhs) const {
        return lhs->level > rhs->level || (lhs->level == rhs->level && lhs->arrivalT > rhs->arrivalT);
    }
};
//TODO: precisin should be same as in test file

void ProcessScheduler::CalculateMean() {
    float sumOfTR = 0, sumOfNT = 0;
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
            FB(1);
            break;
        case 7:
            FB2();
            break;
        case 8:
            Aging(1);
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
    //TODO: or push all processes that arrived first to gether to include case if many processes arrived at 0
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
    priority_queue<Process *, std::vector<Process *>, LessThanByAge> priority_queue;
    priority_queue.push(this->processes.at(0));
    vector<char> result, res;
    int timer = 0, process_idx = 1, qtm = q;
    int arr[this->numOfProcess];
    for (int p = 0; p < this->numOfProcess; p++)
        arr[p] = this->processes.at(p)->serviceT;
    while (timer < this->lastInst) {
        Process *process_temp = priority_queue.top();
        priority_queue.pop();
        qtm = q;
        while (qtm > 0  && process_temp->serviceT > 0) {
            result.emplace_back(process_temp->name);
            res.emplace_back(process_temp->level);
            process_temp->serviceT--;
            qtm--;
            timer++;
            for (int j = process_idx; j < this->numOfProcess; j++) {
                if (this->processes.at(j)->arrivalT > timer) {
                    process_idx = j;
                    break;
                }
                if (this->processes.at(j)->arrivalT == timer) {
                    priority_queue.push(this->processes.at(j));
                    printf("i pushed process %c at level %f\n", this->processes.at(j)->name,
                           this->processes.at(j)->level);
                }
            }
        }

        if (process_temp->serviceT > 0) {
            if (!priority_queue.empty())
                process_temp->level++;
            priority_queue.push(process_temp);
            printf("i pushed process %c at level %f\n", process_temp->name, process_temp->level);
        } else {
            process_temp->finishT = timer;
        }
    }
//printf("\n");
//    for (auto c: result)
//        printf("%c ", c);
//
//printf("end");

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


//    int timer = 0, levels = 1, qtm = q, i, process_idx = 1;
//    vector<void *> vector_of_queues;
//    queue<Process *> first_queue = queue<Process *>();
//    vector<char> results;
//    int arr[this->numOfProcess];
//    for (int p = 0; p < this->numOfProcess; p++) {
//        arr[p] = this->processes.at(p)->serviceT;
//    }
//    int proc_num=0;
//    first_queue.push(this->processes.at(0));
//    vector_of_queues.emplace_back(&first_queue);
//
//    while (timer < this->lastInst) {
//        i = 0;
//        while (i < levels) {
//
//            auto queue_temp = (queue<Process *> *) vector_of_queues.at(i);
//            auto process_temp = queue_temp->front();
//
//            if (!queue_temp->empty() && process_temp->arrivalT <= timer) {
//                queue_temp->pop();
//                while (qtm > 0 && process_temp->serviceT > 0) {
//                    results.emplace_back(process_temp->name);
//                    printf("inserting %c\n", process_temp->name);
//                    process_temp->serviceT--;
//                    qtm--;
//                    timer++;
//                    int p;
//                    for (p = process_idx; p < this->numOfProcess; p++) {
//                        if (this->processes.at(p)->arrivalT > timer) {
//                            break;
//                        }
//                        if (this->processes.at(p)->arrivalT == timer) {
//                            first_queue.push(this->processes.at(p));
//                            proc_num++;
//                            printf("inserting %c\n", process_temp->name);
//                            i=0;
//                        }
//                    }
//                    process_idx = p;
//
//                }
//                if (process_temp->serviceT > 0) {
//                    if (proc_num == 1) {
//                        printf("processing %c\n", process_temp->name);
//                        queue_temp->push(process_temp);
//                    }
//                    else {
//                        queue<Process *> new_queue;
//                        if (levels - i == 1)//i need a new queue
//                        {
//                            vector_of_queues.emplace_back(new queue<Process *>());
//                            levels++;
//                        }
//                        new_queue = *(queue<Process *> *) vector_of_queues.at(i + 1);
//                        new_queue.push(process_temp);
//                        //proc_num++;
//
//                    }
//                } else {
//                    process_temp->finishT = timer;
//                    proc_num--;
//                }
//                qtm = q;
//            } else {
//                i++;
//            }
//        }
//        timer++;
//    }
//    for (int p = 0; p < this->numOfProcess; p++)
//        this->processes.at(p)->serviceT = arr[p];
//
//    for (auto c: results)
//        printf("%c ", c);
//    printf("\n");

}

void ProcessScheduler::FB2() {
    priority_queue<Process *, std::vector<Process *>, LessThanByAge> priority_queue;
    priority_queue.push(this->processes.at(0));
    vector<char> result, res;
    int timer = 0, process_idx = 1, qtm;

    int arr[this->numOfProcess];
    for (int p = 0; p < this->numOfProcess; p++)
        arr[p] = this->processes.at(p)->serviceT;

    while (timer < this->lastInst ) {
        Process *process_temp = priority_queue.top();
        priority_queue.pop();
        qtm = pow(2, process_temp->level);
        while (qtm > 0 &&  process_temp->serviceT > 0) {
            result.emplace_back(process_temp->name);
            res.emplace_back(process_temp->level);
            process_temp->serviceT--;
            qtm--;
            timer++;
            for (int j = process_idx; j < this->numOfProcess; j++) {
                if (this->processes.at(j)->arrivalT > timer) {
                    process_idx = j;
                    break;
                }
                if (this->processes.at(j)->arrivalT == timer) {
                    priority_queue.push(this->processes.at(j));
                    printf("i pushed process %c at level %f\n", this->processes.at(j)->name,
                           this->processes.at(j)->level);
                }
            }
        }

        if (process_temp->serviceT > 0) {
            if (!priority_queue.empty())
                process_temp->level++;
            priority_queue.push(process_temp);
           // printf("i pushed process %c at level %d\n", process_temp->name, process_temp->level);
        } else {
            process_temp->finishT = timer;
        }
    }
//printf("\n");
//    for (auto c: result)
//        printf("%c ", c);
//
//printf("end");

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

void ProcessScheduler::Aging(int q) {
    //
    // Priority     =    ServiceT
    //
    priority_queue<Process *, std::vector<Process *>, LessThanByAge> ready_p;
    q=1;
    std::vector<char> result;
    int timer = this->processes.at(0)->arrivalT, qtm = q, process_idx = 1;
    this->processes.at(0)->level=this->processes.at(0)->serviceT;
    //TODO: or push all processes that arrived first to gether to include case if many processes arrived at 0
    ready_p.push(this->processes.at(0));
    Process *x;
    int arr[this->numOfProcess];
    for (int p = 0; p < this->numOfProcess; p++)
        arr[p] = this->processes.at(p)->serviceT;

    while (!ready_p.empty() && timer < this->lastInst) {
        x = ready_p.top();
        ready_p.pop();
        while (qtm > 0) {
            result.emplace_back(x->name);
            qtm--;
            timer++;
            x->level++;
            for(int p=0; p<this->numOfProcess; p++){
                if(this->processes.at(p)->arrivalT<=timer)
                    this->processes.at(p)->level--;
            }
                
            for (int j = process_idx; j < this->numOfProcess; j++) {
                if (this->processes.at(j)->arrivalT > timer) {
                    process_idx = j;
                    break;
                }
                if (this->processes.at(j)->arrivalT == timer) {
                    this->processes.at(j)->level--;
                    ready_p.push(this->processes.at(j));
                }
            }

        }
        //if (x->serviceT > 0)
        ready_p.push(x);
        //else
            //x->finishT = timer;
        qtm = q;
    }

    for (int p = 0; p < this->numOfProcess; p++)
        this->processes.at(p)->serviceT = arr[p];

    for (auto p: this->processes) {
        for (int i = 0; i < p->arrivalT; i++) {
            printf(" |");
        }

        for (int i = p->arrivalT; i < this->lastInst; i++) {
            if (result.at(i) != p->name) {
                printf(".|");
            } else {
                printf("*|");
            }
        }


        // for (int i = p->finishT; i < this->lastInst; i++)
        //     printf(" |");

        // p->turnRT = p->finishT - p->arrivalT;
        // p->normT = (p->turnRT * 1.0) / p->serviceT;
        printf("\n");
    }
    for(char c: result)
        printf("%c ", c);
    printf("\n");
    // CalculateMean();
    // for (auto x: this->processes) {
    //     printf("%c %d %d %d %d %.2f\n", x->name, x->arrivalT, x->serviceT, x->finishT, x->turnRT, x->normT);
    // }
    // printf(" meanTurnR= %f meanNormT=%f\n", this->meanTurnR, this->meanNormT);

}

void ProcessScheduler::SPN() {
    int wait = 0, timer=0;
    priority_queue<Process *, std::vector<Process *>, LessThanByAge> priority_queue;
    Process *x;
    vector <char> result;
    this->processes.at(0)->level=this->processes.at(0)->serviceT;
    priority_queue.push(this->processes.at(0));
    x = priority_queue.top();
    priority_queue.pop();

    for (int j = 1; j <= this->numOfProcess; j++) {
        timer+=x->serviceT;
        x->finishT=timer;
        //x->finishT = wait + x->serviceT;
        x->turnRT = x->finishT - x->arrivalT;
        x->normT = (x->turnRT * 1.0) / x->serviceT;
        //CalculateMean();
        wait = x->serviceT + wait;
        // printf("\n");
        
        //printf("%d\n", timer);
        for(int i=j; i<this->numOfProcess; i++){
            if(priority_queue.size()==this->numOfProcess-j)
                break;
            if(this->processes.at(i)->arrivalT <= timer){
                this->processes.at(i)->level=this->processes.at(i)->serviceT;
                priority_queue.push(this->processes.at(i));
            }
        }
        for(int i=0;i<x->serviceT;i++)
            result.emplace_back(x->name);
        x = priority_queue.top();
        priority_queue.pop();
        
    }
    for(char res:result)
        printf("%c ", res);
        printf("\n");
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

void ProcessScheduler::SRT() {
        //TODO: meanNormT=2.00 != 2.54 recheck CalculateMean()
    priority_queue<Process *, std::vector<Process *>, LessThanByAge> ready_p;
    std::vector<char> result;
    int timer = this->processes.at(0)->arrivalT, qtm = 1, process_idx = 1;
    //TODO: or push all processes that arrived first to gether to include case if many processes arrived at 0
    this->processes.at(0)->level=this->processes.at(0)->serviceT;
    ready_p.push(this->processes.at(0));
    Process *x;
    int arr[this->numOfProcess];
    for (int p = 0; p < this->numOfProcess; p++)
        arr[p] = this->processes.at(p)->serviceT;

    while (!ready_p.empty() && timer < this->lastInst) {
        x = ready_p.top();
        ready_p.pop();
        while (qtm > 0 && x->serviceT > 0) {
            result.emplace_back(x->name);
            qtm--;
            timer++;
            x->serviceT--;
            x->level--;

            for (int j = process_idx; j < this->numOfProcess; j++) {
                if (this->processes.at(j)->arrivalT > timer) {
                    process_idx = j;
                    break;
                }
                if (this->processes.at(j)->arrivalT == timer) {
                    this->processes.at(j)->level=this->processes.at(j)->serviceT;
                    ready_p.push(this->processes.at(j));
                }
            }

        }
        if (x->serviceT > 0)
            ready_p.push(x);
        else
            x->finishT = timer;
        qtm = 1;
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

void ProcessScheduler::HRRN() {
     int wait = 0, timer=0;
    priority_queue<Process *, std::vector<Process *>, LessThanByAge> priority_queue;
    Process *x;
    vector <char> result;
    this->processes.at(0)->level=this->processes.at(0)->serviceT;
    priority_queue.push(this->processes.at(0));
    x = priority_queue.top();
    priority_queue.pop();

    for (int j = 1; j <= this->numOfProcess; j++) {
        timer+=x->serviceT;
        x->finishT=timer;
        //x->finishT = wait + x->serviceT;
        x->turnRT = x->finishT - x->arrivalT;
        x->normT = (x->turnRT * 1.0) / x->serviceT;
        //CalculateMean();
        wait = x->serviceT + wait;
        // printf("\n");
        
        //printf("%d\n", timer);
        for(int i=j; i<this->numOfProcess; i++){
            if(priority_queue.size()==this->numOfProcess-j)
                break;
            if(this->processes.at(i)->arrivalT <= timer){
                this->processes.at(i)->level=this->processes.at(i)->serviceT/this->processes.at(i)->serviceT;
                priority_queue.push(this->processes.at(i));
            }
        }
        for(int i=j; i<this->numOfProcess;i++){
            if(this->processes.at(i)->arrivalT<timer){
            this->processes.at(i)->wait = timer - this->processes.at(i)->arrivalT;
            this->processes.at(i)->level = (1.0*this->processes.at(i)->serviceT+this->processes.at(i)->wait)/(1.0*this->processes.at(i)->serviceT);
            }

        }
        for(int i=0;i<x->serviceT;i++)
            result.emplace_back(x->name);
        x = priority_queue.top();
        priority_queue.pop();
        
    }
    for(char res:result)
        printf("%c ", res);
        printf("\n");
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

