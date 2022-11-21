//
//  main.cpp
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

#include <bits/stdc++.h>
#include <string.h>
#include <iostream>
#include "ProcessScheduler.hpp"
#include <vector>

using namespace std;

vector<string> mystrtok(string str, char delim) {
    vector<string> tokens;
    string temp = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == delim) {
            tokens.push_back(temp);
            temp = "";
        } else
            temp += str[i];
    }
    tokens.push_back(temp);
    return tokens;
}


struct policy_quantum {
    int pno;
    int qval = 0;
};

int main() {
    //scanning input
    string line1, line2, line3, line4, line5;
    cin >> line1;
    cin >> line2;
    cin >> line3;
    cin >> line4;

    vector<policy_quantum *> policies_quantums;
    vector<string> pq;
    vector<string> policies = mystrtok(line2, ',');
    int police[200];
    int last_inst = stoi(line3);
    int proc_no = stoi(line4);

    for (string token: policies) {
        pq = mystrtok(token, '-');
        policy_quantum *p_q = new policy_quantum();
        if (pq.size() == 1) {
            p_q->pno = stoi(token);
        } else {
            p_q->pno = stoi(pq[0]);
            p_q->qval = stoi(pq[1]);
        }
        policies_quantums.emplace_back(p_q);
    }
    vector<string> l5;
    cin >> line5;
    cin.ignore();
    char pname[1];
    int arrT, serT, i;
    vector<Process *> processes;
    while (!line5.empty()) {
        l5 = mystrtok(line5, ',');
        strcpy(pname,l5[0].c_str());
        processes.emplace_back(new Process(stoi(l5[1]), stoi(l5[2]),pname[0]));
        getline(cin, line5);
    }

    for (auto x : policies_quantums) {
        ProcessScheduler scheduler(x->pno, line1.c_str(), last_inst, proc_no, x->qval);
        scheduler.processes = processes;
        scheduler.StartScheduler();
        for(auto p:processes)
            p->level = 0;
    }
    return 0;
}