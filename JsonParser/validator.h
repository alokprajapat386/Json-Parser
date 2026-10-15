
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include<stdlib.h>
#include<string.h>

#define true 1
#define false 0



int transition[9][5]={
    {2, 1, 3, -1, -1},
    {-1, -1, -1, 4, 6},
    {-1, 1, 3, -1, -1},
    {-1, 3, 3, 4, 6 },
    {-1, 5, 5, -1, -1},
    {-1, 5, 5, -1, 6},
    {7, 8, 8, -1, -1},
    {-1, 8, 8, -1, -1},
    {-1, 8, 8, -1, -1},
};

int transition_var(char ch){
    if(ch=='+' || ch=='-') return 0;
    if(ch=='0') return 1;
    if(ch>='1' && ch<='9') return 2;
    if(ch=='.') return 3;
    if(ch=='e' || ch=='E') return 4;
    return -1;
}

int validate_number(char* s, int len){
    int initial_state=0;
    
    if(len>0 && s[0]=='+') return 0;
    for(int i=0; i<len; i++){

        int transition_index=transition_var(s[i]);
        if(transition_index==-1) return 0;
        initial_state=transition[initial_state][transition_index];
        if(initial_state==-1) return 0;
    }
    return 1;
}

int are_equal(char*s , int slen, char* t, int tlen){
    if(slen!=tlen) return 0;
    for(int i=0; i<slen; i++){
        if(s[i]==t[i]) return false;
    }
    return true;
}

int is_boolean(char*s, int len){
    char* curr="true";
    if(are_equal(s, len, curr, 4)) return true;
    curr="false";
    if(are_equal(s, len, curr, 5)) return true;
    return false;
}

int is_null(char* s, int len){
    char* curr="null";
    if(are_equal(s, len, curr, 4)) return true;
    return false;
}




#endif