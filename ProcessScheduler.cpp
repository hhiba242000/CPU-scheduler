//
//  ProcessScheduler.cpp
//  CPU scheduling
//
//  Created by Hayam Hiba on 11/11/2022.
//  Made in collaboration with Adel Yasser

//1. FCFS (First Come First Serve)
//2. RR (Round Robin)
//3. SPN (Shortest Process Next)
//4. SRT (Shortest Remaining Time)
//5. HRRN (Highest Response Ratio Next)
//6. FB-1, (Feedback where all queues have q=1)
//7. FB-2i, (Feedback where q= 2i)
//8. Aging

#include "ProcessScheduler.hpp"
#include <queue>
#include <set>
#include <cmath>

using namespace std;

// Overriding structures for the priority queues ordering used depending on the policy used.
struct LessThanByLevel {
    bool operator()(const Process *lhs, const Process *rhs) const {
        return lhs->level > rhs->level || (lhs->level == rhs->level && lhs->arrivalT > rhs->arrivalT);
    }
};

struct LessThanBySRT {
    bool operator()(const Process *lhs, const Process *rhs) const {
        return lhs->level > rhs->level || (lhs->level == rhs->level && lhs->lastPush > rhs->lastPush);
    }
};

struct LessThanByAging {
    bool operator()(const Process *lhs, const Process *rhs) const {
        return lhs->level < rhs->level || (lhs->level == rhs->level && lhs->lastPush > rhs->lastPush);
    }
};

// Function to calculate mean by dividing the sum by the number
void ProcessScheduler::CalculateMean() {
    float sumOfTR = 0, sumOfNT = 0;
    for (auto x: this->processes) {
        sumOfNT += x->normT;
        sumOfTR += x->turnRT;
    }
    this->meanTurnR = sumOfTR / (1.0 * this->numOfProcess);
    this->meanNormT = sumOfNT / (1.0 * this->numOfProcess);
}

// Function to print any policy result. '.' indicates wait and '*' indicates active.
void ProcessScheduler::PrintTraceResults() {
    for (int i = 0; i < this->lastInst + 1; i++) printf("%d ", i % (this->lastInst / 2));
    printf("\n");
    for (int i = 0; i < 8 + 2 * this->lastInst; i++) printf("-");
    printf("\n");
    for (auto p: this->processes) {
        printf("%-6c", p->name);
        for (int i = 0; i < p->arrivalT; i++) {
            printf("| ");
        }

        for (int i = p->arrivalT; i < p->finishT; i++) {
            if (results.at(i) != p->name) {
                printf("|.");
            } else {
                printf("|*");
            }
        }

        for (int i = p->finishT; i < this->lastInst; i++)
            printf("| ");
        printf("| \n");

    }
    for (int i = 0; i < 8 + 2 * this->lastInst; i++) printf("-");
    printf("\n");
    printf("\n");
}

// Function to print all required stats as in format
void ProcessScheduler::PrintStatsResults() {
    char separator = '|';
    for (auto p: this->processes) {
        p->turnRT = p->finishT - p->arrivalT;
        p->normT = (p->turnRT * 1.0) / p->serviceT;
    }
    CalculateMean();
    printf("\nProcess    |");
    for (auto x: this->processes) {
        printf("%3c%3c", x->name, separator);
    }

    printf("\nArrival    |");
    for (auto x: this->processes) {
        printf("%3d%3c", x->arrivalT, separator);
    }

    printf("\nService    |");
    for (auto x: this->processes) {
        printf("%3d%3c", x->serviceT, separator);
    }
    printf(" Mean|");

    printf("\nFinish     |");
    for (auto x: this->processes) {
        printf("%3d%3c", x->finishT, separator);
    }
    printf("-----|");

    printf("\nTurnaround |");
    for (auto x: this->processes) {
        printf("%3d%3c", x->turnRT, separator);
    }
    printf("%5.2f|", this->meanTurnR);

    printf("\nNormTurn   |");
    for (auto x: this->processes) {
        printf("%5.2f|", x->normT);
    }
    printf("%5.2f|\n", this->meanNormT);
    printf("\n");

}

// Start Scheduler depending on policy and quantum given.
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
            FB();
            break;
        case 7:
            FB2();
            break;
        case 8:
            Aging(this->quantum);
            break;
        default:
            printf("Error parsing input file");
            exit(1);
    }
}

void ProcessScheduler::FCFSSchedule() {
    int wait;
    if (this->display == "trace") {
        printf("FCFS  ");
        for (int i = 0; i < this->lastInst + 1; i++) printf("%d ", i % (this->lastInst / 2));
        printf("\n");
        for (int i = 0; i < 8 + 2 * this->lastInst; i++) printf("-");
        printf("\n");

        // Loop over every process that runs till it finishes execution. first come first serve.
        wait = 0;
        Process *x;
        for (int j = 0; j < this->numOfProcess; j++) {
            x = this->processes.at(j);
            printf("%-6c", x->name);
            for (int i = 0; i < x->arrivalT; i++) {
                *(this->traceDisplay + j * this->numOfProcess + i) = '*';
                printf("| ");
            }

            for (int i = x->arrivalT; i < wait; i++) {
                *(this->traceDisplay + j * this->numOfProcess + i) = '.';
                printf("|.");
            }

            for (int i = 0; i < x->serviceT; i++) {
                *(this->traceDisplay + j * this->numOfProcess + i) = '*';
                printf("|*");
            }

            for (int i = wait + x->serviceT; i < this->lastInst; i++) {
                *(this->traceDisplay + j * this->numOfProcess + i) = ' ';
                printf("| ");
            }
            x->finishT = wait + x->serviceT;
            wait = x->serviceT + wait;
            printf("| \n");
        }
        for (int i = 0; i < 8 + 2 * this->lastInst; i++) printf("-");
        printf("\n");
        printf("\n");
    }
    if (this->display == "stats") {
        printf("FCFS");
        wait = 0;
        for (auto x: this->processes) {
            x->finishT = wait + x->serviceT;
            wait = x->serviceT + wait;
        }
        PrintStatsResults();
    }
    // make sure processes are sorted by arrival time (confirmed on piazza)

}

void ProcessScheduler::RRSchedule(int q) {
    // Standard queue is required for each process since we have no priorities.
    std::queue<Process *> ready_p;
    int timer = this->processes.at(0)->arrivalT, qtm = q, process_idx = 1;
    int min_arrivali_time = this->processes.at(0)->arrivalT;
    for (auto prc: this->processes)
        if (prc->arrivalT == min_arrivali_time)
            ready_p.push(this->processes.at(0));

    Process *x;
    int arr[this->numOfProcess];
    for (int p = 0; p < this->numOfProcess; p++)
        arr[p] = this->processes.at(p)->serviceT;

    // Loop until last instant is reached. pop the front of the queue. process it for quantum time and push it back into the queue.
    while (!ready_p.empty() && timer < this->lastInst) {
        x = ready_p.front();
        ready_p.pop();
        while (qtm > 0 && x->serviceT > 0) {
            results.emplace_back(x->name);
            qtm--;
            timer++;
            x->serviceT--;

            for (int j = process_idx; j < this->numOfProcess; j++) {
                if (this->processes.at(j)->arrivalT > timer) {
                    process_idx = j;
                    break;
                }
                // if a process arrives now, push it in the queue
                if (this->processes.at(j)->arrivalT == timer) {
                    ready_p.push(this->processes.at(j));
                }
            }

        }
        // Push it in the queue if it didn't finish, else it ended so save finish time
        if (x->serviceT > 0)
            ready_p.push(x);
        else
            x->finishT = timer;
        qtm = q;
    }

    for (int p = 0; p < this->numOfProcess; p++)
        this->processes.at(p)->serviceT = arr[p];

    if (this->display == "trace") {
        printf("RR-%d  ", this->quantum);
        PrintTraceResults();
    }
    if (this->display == "stats") {
        printf("RR-%d", this->quantum);
        PrintStatsResults();
    }


}

void ProcessScheduler::FB() {
    // Priority queue is used to represent the feedback algorithm. Processes with higher level have lower priority and are served after.
    // Remember, a priority queue is considered a queue of queues for each processes with the same priority value.
    // Hence, a priority queue fits perfectly in this implementation and simplifies it.
    priority_queue<Process *, std::vector<Process *>, LessThanByLevel> priority_queue;
    vector<char> result, res;
    int qtm=1;

    int min_arrivali_time = this->processes.at(0)->arrivalT;
    for (auto prc: this->processes)
        if (prc->arrivalT == min_arrivali_time)
            priority_queue.push(this->processes.at(0));

    int timer = 0, process_idx = 1;
    int arr[this->numOfProcess];
    for (int p = 0; p < this->numOfProcess; p++){
        arr[p] = this->processes.at(p)->serviceT;
        this->processes.at(p)->level=0;
        }
    // Loop until we reach last instant
    while (timer < this->lastInst) {
        // Pop the first in priority queue and process it
        Process *process_temp = priority_queue.top();
        priority_queue.pop();

        while (qtm > 0 && process_temp->serviceT > 0) {
            this->results.emplace_back(process_temp->name);
            res.emplace_back(process_temp->level);
            process_temp->serviceT--;
            qtm--;
            timer++;
            for (int j = process_idx; j < this->numOfProcess; j++) {
                if (this->processes.at(j)->arrivalT > timer) {
                    process_idx = j;
                    break;
                }
                // If a process enters now. set its level and push it
                if (this->processes.at(j)->arrivalT == timer) {
                    priority_queue.push(this->processes.at(j));
                    this->processes.at(j)->level;
                }
            }
        }
        // handle current process according to remaining time
        qtm=1;
        if (process_temp->serviceT > 0) {
            if (!priority_queue.empty())
                process_temp->level++;
            priority_queue.push(process_temp);
        } else {
            process_temp->finishT = timer;
        }
    }

    for (int p = 0; p < this->numOfProcess; p++)
        this->processes.at(p)->serviceT = arr[p];


    if (this->display == "trace") {
        printf("FB-1  ");
        PrintTraceResults();
    }
    if (this->display == "stats") {
        printf("FB-1");
        PrintStatsResults();
    }

}

void ProcessScheduler::FB2() {
    // Similar to FB, however quantum is 2^i depending on the queue which the process is placed (its level in the priority queue.)
    priority_queue<Process *, std::vector<Process *>, LessThanByLevel> priority_queue;

    vector<char> res;
    int timer = 0, process_idx = 1, qtm;

    int min_arrivali_time = this->processes.at(0)->arrivalT;
    for (auto prc: this->processes)
        if (prc->arrivalT == min_arrivali_time)
            priority_queue.push(this->processes.at(0));

    int arr[this->numOfProcess];
    for (int p = 0; p < this->numOfProcess; p++){
        arr[p] = this->processes.at(p)->serviceT;
        this->processes.at(p)->level=0;
        }

    while (timer < this->lastInst) {
        Process *process_temp = priority_queue.top();
        priority_queue.pop();
        qtm = pow(2, process_temp->level);
        while (qtm > 0 && process_temp->serviceT > 0) {
            this->results.emplace_back(process_temp->name);
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
                    this->processes.at(j)->level;
                }
            }
        }

        if (process_temp->serviceT > 0) {
            if (!priority_queue.empty())
                process_temp->level++;
            priority_queue.push(process_temp);
        } else {
            process_temp->finishT = timer;
        }
    }


    for (int p = 0; p < this->numOfProcess; p++)
        this->processes.at(p)->serviceT = arr[p];

    if (this->display == "trace") {
        printf("FB-2i ");
        PrintTraceResults();
    }
    if (this->display == "stats") {
        printf("FB-2i");
        PrintStatsResults();
    }

}

void ProcessScheduler::Aging(int q) {
    //
    // Priority     =    ServiceT (Based on our tokenization and for simplicity)
    //
    // priority queue here is based on the priority + age of process.
    // Here higher level is higher priority.
    priority_queue<Process *, std::vector<Process *>, LessThanByAging> ready_p;
    std::vector<char> result;
    int timer = this->processes.at(0)->arrivalT, qtm = q, process_idx = 1;
    this->processes.at(0)->level = this->processes.at(0)->serviceT;
    int min_arrivali_time = this->processes.at(0)->arrivalT;
    for (auto prc: this->processes)
        if (prc->arrivalT == min_arrivali_time)
            ready_p.push(this->processes.at(0));
    Process *x;
    int arr[this->numOfProcess];
    for (int p = 0; p < this->numOfProcess; p++)
        arr[p] = this->processes.at(p)->serviceT;
    // loop until last instant
    while (!ready_p.empty() && timer < this->lastInst) {
        // pop the first in queue. return its priority to initial priority and age every process that is in the queue
        x = ready_p.top();
        ready_p.pop();
        while (qtm > 0) {
            result.emplace_back(x->name);
            x->level = x->serviceT;
            qtm--;
            timer++;
            x->level--;
            for (int p = 0; p < this->numOfProcess; p++) {
                if (this->processes.at(p)->arrivalT < timer) {
                    this->processes.at(p)->level++;
                }
            }

            for (int j = process_idx; j < this->numOfProcess; j++) {
                if (this->processes.at(j)->arrivalT > timer) {
                    process_idx = j;
                    break;
                }
                // add newly arrived processes with their respective level and their time of adding to the queue
                if (this->processes.at(j)->arrivalT == timer) {
                    this->processes.at(j)->level = this->processes.at(j)->serviceT + 1;
                    this->processes.at(j)->lastPush=timer;
                    ready_p.push(this->processes.at(j));
                }
            }
        }
        // return current process to priority queue with its time of adding to queue.
        x->lastPush=timer;
        ready_p.push(x);
        qtm = q;
    }

    for (int p = 0; p < this->numOfProcess; p++)
        this->processes.at(p)->serviceT = arr[p];

    printf("Aging ");
    for (int i = 0; i < this->lastInst + 1; i++) printf("%d ", i % (this->lastInst / 2));
    printf("\n");
    for (int i = 0; i < 8 + 2 * this->lastInst; i++) printf("-");
    printf("\n");
    for (auto p: this->processes) {
        printf("%-6c", p->name);
        for (int i = 0; i < p->arrivalT; i++) {
            printf("| ");
        }

        for (int i = p->arrivalT; i < this->lastInst; i++) {
            if (result.at(i) != p->name) {
                printf("|.");
            } else {
                printf("|*");
            }
        }

        printf("| \n");
    }
    for (int i = 0; i < 8 + 2 * this->lastInst; i++) printf("-");
   printf("\n");
    printf("\n");


}

void ProcessScheduler::SPN() {
    // Non-preemtive. add to queue and have the service time decide the priority in priority queue.
    int wait = 0, timer = 0;
    priority_queue<Process *, std::vector<Process *>, LessThanByLevel> priority_queue;
    Process *x;
    this->processes.at(0)->level = this->processes.at(0)->serviceT;
    int min_arrivali_time = this->processes.at(0)->arrivalT;
    for (auto prc: this->processes)
        if (prc->arrivalT == min_arrivali_time)
            priority_queue.push(this->processes.at(0));

    x = priority_queue.top();
    priority_queue.pop();
    // Loop over every process according to arrival time
    for (int j = 1; j <= this->numOfProcess; j++) {
        timer += x->serviceT;
        x->finishT = timer;
        x->turnRT = x->finishT - x->arrivalT;
        x->normT = (x->turnRT * 1.0) / x->serviceT;
        wait = x->serviceT + wait;

        for (int i = j; i < this->numOfProcess; i++) {
            if (priority_queue.size() == this->numOfProcess - j)
                break;
            if (this->processes.at(i)->arrivalT <= timer) {
                this->processes.at(i)->level = this->processes.at(i)->serviceT;
                priority_queue.push(this->processes.at(i));
            }
        }
        for (int i = 0; i < x->serviceT; i++)
            this->results.emplace_back(x->name);
        x = priority_queue.top();
        priority_queue.pop();

    }

    if (this->display == "trace") {
        printf("SPN   ");
        PrintTraceResults();
    }
    if (this->display == "stats") {
        printf("SPN");
        PrintStatsResults();
    }

}

void ProcessScheduler::SRT() {
    // Preemptive version of SPN. very similar but each process runs for a quantum.
    priority_queue<Process *, std::vector<Process *>, LessThanBySRT> ready_p;
    int timer = this->processes.at(0)->arrivalT, qtm = 1, process_idx = 1;
    this->processes.at(0)->level = this->processes.at(0)->serviceT;
    int min_arrivali_time = this->processes.at(0)->arrivalT;
    for (auto prc: this->processes)
        if (prc->arrivalT == min_arrivali_time)
            ready_p.push(this->processes.at(0));

    Process *x;
    int arr[this->numOfProcess];
    for (int p = 0; p < this->numOfProcess; p++)
        arr[p] = this->processes.at(p)->serviceT;

    while (!ready_p.empty() && timer < this->lastInst) {
        x = ready_p.top();
        ready_p.pop();
        while (qtm > 0 && x->serviceT > 0) {
            this->results.emplace_back(x->name);
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
                    this->processes.at(j)->level = this->processes.at(j)->serviceT;

                    this->processes.at(j)->lastPush=timer;
                    ready_p.push(this->processes.at(j));
                }
            }

        }

        x->lastPush=timer;
        if (x->serviceT > 0)
            ready_p.push(x);
        else
            x->finishT = timer;
        qtm = 1;
    }

    for (int p = 0; p < this->numOfProcess; p++)
        this->processes.at(p)->serviceT = arr[p];

    if (this->display == "trace") {
        printf("SRT   ");
        PrintTraceResults();
    }
    if (this->display == "stats") {
        printf("SRT");
        PrintStatsResults();
    }
}

void ProcessScheduler::HRRN() {
    // very similar to SPN however highest response ratio is the level of priority queue
    int wait = 0, timer = 0;
    priority_queue<Process *, std::vector<Process *>, LessThanByLevel> priority_queue;
    Process *x;
    this->processes.at(0)->level = this->processes.at(0)->serviceT;
    int min_arrivali_time = this->processes.at(0)->arrivalT;
    for (auto prc: this->processes)
        if (prc->arrivalT == min_arrivali_time)
            priority_queue.push(this->processes.at(0));
    x = priority_queue.top();
    priority_queue.pop();

    for (int j = 1; j <= this->numOfProcess; j++) {
        timer += x->serviceT;
        x->finishT = timer;
        //x->finishT = wait + x->serviceT;
        x->turnRT = x->finishT - x->arrivalT;
        x->normT = (x->turnRT * 1.0) / x->serviceT;
        //CalculateMean();
        wait = x->serviceT + wait;
        // printf("\n");

        //printf("%d\n", timer);
        for (int i = j; i < this->numOfProcess; i++) {
            if (priority_queue.size() == this->numOfProcess - j)
                break;
            if (this->processes.at(i)->arrivalT <= timer) {
                this->processes.at(i)->level = this->processes.at(i)->serviceT / this->processes.at(i)->serviceT;
                priority_queue.push(this->processes.at(i));
            }
        }
        for (int i = j; i < this->numOfProcess; i++) {
            if (this->processes.at(i)->arrivalT < timer) {
                this->processes.at(i)->wait = timer - this->processes.at(i)->arrivalT;
                this->processes.at(i)->level = (1.0 * this->processes.at(i)->serviceT + this->processes.at(i)->wait) /
                                               (1.0 * this->processes.at(i)->serviceT);
            }

        }
        for (int i = 0; i < x->serviceT; i++)
            this->results.emplace_back(x->name);
        x = priority_queue.top();
        priority_queue.pop();

    }

    if (this->display == "trace") {
        printf("HRRN  ");
        PrintTraceResults();
    }
    if (this->display == "stats") {
        printf("HRRN");
        PrintStatsResults();
    }

}



