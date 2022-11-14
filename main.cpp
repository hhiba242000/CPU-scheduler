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


vector<string> mystrtok(string str, char delim){
    vector<string> tokens;
    string temp = "";
    for(int i = 0; i < str.length(); i++){
        if(str[i] == delim){
            tokens.push_back(temp);
            temp = "";
        }
        else
            temp += str[i];           
    }
    tokens.push_back(temp);
    return tokens;
}


int main(){
    //scanning input
    string line1,line2,line3,line4,line5;
    cin >> line1;
    cin >> line2;
    cin >> line3;
    cin >> line4;
    //line 1       
    //line 2
    int pno;
    int qval=0;
    int odd=0;
    vector <string> pq;
    vector<string> policies = mystrtok(line2, ',');
    for(string line2: policies)
        {
            pq = mystrtok(line2,'-');
            for(string policy : pq )
            {
                // to be implemented, process multiple scheduling types later
                if(pq.size()==1)
                    pno = stoi(policy);
                else {
                    if(odd==0)
                        pno = stoi(policy);
                    else
                        qval = stoi(policy);
                }
                odd++;
                //cout << policy << endl;
            }
        }
    //line 3
    int last_inst = stoi(line3);
    //line 4
    int proc_no = stoi(line4);

    ProcessScheduler scheduler( pno, line1.c_str(), last_inst, proc_no, qval );

    vector <string> l5 ;
    cin >> line5; 
    cin.ignore();
    char pname[1];
    int arrT, serT, i;

    while(!empty(line5)){
        i=0;
        l5 = mystrtok(line5, ',');
        for(string token5: l5){
            if(i==0) strcpy(pname,token5.c_str());
            else if(i==1) arrT=stoi(token5);
            else serT=stoi(token5);
            i++;
        }
        scheduler.processes.emplace_back(new Process(arrT,serT,pname[0]));
        getline(cin,line5);   
    }
    // int arrivalT;
    // int serviceT;
    // int finishT;
    // int turnRT;
    // double normT;

    scheduler.StartScheduler();
    // for(auto x:scheduler.processes){
    //     printf("%c %d %d %d %d %.2f\n",x->name.c_str(),x->arrivalT,x->serviceT,x->finishT,x->turnRT,x->normT);
    // }
    // printf(" meanTurnR= %f meanNormT=%f\n",scheduler.meanTurnR,scheduler.meanNormT);
    return 0;
}
    // for (Process temp: processes){
    //     scheduler.processes.emplace_back(processes.back());
    //     processes.pop_back();
    // }
    // Process p1("A",0,1);
    // p1.insert("X",0,3);
    // Process p2("B",2,6);
    // Process p3("C",4,4);
    // Process p4("D",6,5);
    // Process p5("E",8,2);

    
    // scheduler.processes.emplace_back(&p1);
    // scheduler.processes.emplace_back(&p2);
    // scheduler.processes.emplace_back(&p3);
    // scheduler.processes.emplace_back(&p4);
    // scheduler.processes.emplace_back(&p5);

//trace
//1
//20
//5
//A,0,3
//B,2,6
//C,4,4
//D,6,5
//E,8,2