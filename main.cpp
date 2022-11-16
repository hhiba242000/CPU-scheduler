//
//  main.cpp
//  CPU scheduling
//
//  Created by Hayam Hiba on 11/11/2022.
//
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "ProcessScheduler.hpp"
#include <vector>

using namespace std;


//vector<string> mystrtok(string str, char delim) {
//    vector<string> tokens;
//    string temp = "";
//    for (int i = 0; i < str.length(); i++) {
//        if (str[i] == delim) {
//            tokens.push_back(temp);
//            temp = "";
//        } else
//            temp += str[i];
//    }
//    tokens.push_back(temp);
//    return tokens;
//}

struct LessThanBylevel {
    bool operator()(const Process *lhs, const Process *rhs) const {
        return lhs->level > rhs->level;
    }
};

int main(){

    Process p1(0,3,'A');
    Process p2(2,6,'B');
    Process p3(4,4,'C');
    Process p4(6,5,'D');
    Process p5(8,2,'E');

    ProcessScheduler scheduler(3,"trace",20,5,1);
    scheduler.processes.emplace_back(&p1);
    scheduler.processes.emplace_back(&p2);
    scheduler.processes.emplace_back(&p3);
    scheduler.processes.emplace_back(&p4);
    scheduler.processes.emplace_back(&p5);
    scheduler.StartScheduler();

    //scanning input
//    string line1,line2,line3,line4,line5;
//    cin >> line1;
//    cin >> line2;
//    cin >> line3;
//    cin >> line4;
//    //line 1
//    //line 2
//    int pno;
//    int qval=0;
//    int odd=0;
//    vector <string> pq;
//    vector<string> policies = mystrtok(line2, ',');
//    for(string line2: policies)
//        {
//            pq = mystrtok(line2,'-');
//            for(string policy : pq )
//            {
//                // to be implemented, process multiple scheduling types later
//                if(pq.size()==1)
//                    pno = stoi(policy);
//                else {
//                    if(odd==0)
//                        pno = stoi(policy);
//                    else
//                        qval = stoi(policy);
//                }
//                odd++;
//                //cout << policy << endl;
//            }
//        }
//    //line 3
//    int last_inst = stoi(line3);
//    //line 4
//    int proc_no = stoi(line4);
//    printf("%d %s %d %d %d\n",pno, line1.c_str(), last_inst, proc_no, qval);
//    ProcessScheduler scheduler( pno, line1.c_str(), last_inst, proc_no, 1 );
//
//    vector <string> l5 ;
//    cin >> line5;
//    cin.ignore();
//    char pname[1];
//    int arrT, serT, i;
//
//    while(!line5.empty()){
//        i=0;
//        l5 = mystrtok(line5, ',');
//        for(string token5: l5){
//            if(i==0) strcpy(pname,token5.c_str());
//            else if(i==1) arrT=stoi(token5);
//            else serT=stoi(token5);
//            i++;
//        }
//        scheduler.processes.emplace_back(new Process(arrT,serT,pname[0]));
//        printf("im here");
//        getline(cin,line5);
//    }
//    printf("im here2");
//    for(auto p: scheduler.processes)
//        printf("name: %c ",p->name);
//
//    scheduler.StartScheduler();

    return 0;
}