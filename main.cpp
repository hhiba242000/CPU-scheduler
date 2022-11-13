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

int main(int argc, const char * argv[]) {
    // insert code here...

    while(true){
        //line 1
        string line1,line2,line3,line4,line5;
        getline(cin,line1);
        getline(cin,line2);
        getline(cin,line3);
        getline(cin,line4);
        vector <string> l5 ;
        getline(cin,line5);
        int i=0;
        while(!empty(line5)){
            l5 = mystrtok(line5, ',');
            for(string line5: l5)
            cout << line5 << endl;
            getline(cin,line5);
        }
        cout << line1 << "\n";
        //line 2
        vector<string> policies = mystrtok(line2, ',');
        for(string line2: policies)
            cout << line2 << endl;
        //line 3
        cout << line3 << "\n";
        //line 4
        cout << line4 << "\n";
        //line 5
        // for(string line5: l5)
        //     cout << line5 << endl;
}

    return 0;
}

int main(){
    Process p1(0,3,"A");
    Process p2(2,6,"B");
    Process p3(4,4,"C");
    Process p4(6,5,"D");
    Process p5(8,2,"E");

    ProcessScheduler scheduler(1,"trace",20,5,0);
    scheduler.processes.emplace_back(&p1);
    scheduler.processes.emplace_back(&p2);
    scheduler.processes.emplace_back(&p3);
    scheduler.processes.emplace_back(&p4);
    scheduler.processes.emplace_back(&p5);

    int arrivalT;
    int serviceT;
    int finishT;
    int turnRT;
    double normT;

    scheduler.StartScheduler();
    for(auto x:scheduler.processes){
        printf("%s %d %d %d %d %.2f\n",x->name.c_str(),x->arrivalT,x->serviceT,x->finishT,x->turnRT,x->normT);
    }
    printf(" meanTurnR= %f meanNormT=%f\n",scheduler.meanTurnR,scheduler.meanNormT);
//    scheduler.PrintSchedule("trace");
    return 0;
}
//trace
//1
//20
//5
//A,0,3
//B,2,6
//C,4,4
//D,6,5
//E,8,2