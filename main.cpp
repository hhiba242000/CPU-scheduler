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
//#include "Process.hpp"
//#include "ProcessScheduler.hpp"
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
