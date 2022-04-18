#pragma once

#include <cstdlib>

// Not to be confused with Node.type
enum NodeReturnType{
    // BOOL, //Maybe later, it complicates things
    INT,
    FLOAT,
    NONE //returned by getParam if nothing is taken at that position
};

class Node{
public:
    //00: function, 01: const, 10: int input, 11: float input
    unsigned char type :2;
    //either input index or function number
    unsigned char val  :6;

    enum NodeReturnType getParam1();
    enum NodeReturnType getParam2();

    unsigned char fullVal(){
        return (type << 6) & val;
    };
};

/* function Node.val meanings:
    3rd highest bit (32) indicates return type

    00 : NULL (end of tree)
INT:
    01 (0) : +(INT, INT)
    02 (1) : -(INT, INT)
    03 (2) : *(INT, INT)
    04 (3) : /(INT, INT)
    05 (4) : mod(INT, INT)
    06 (5) : round(FLOAT)
FLOAT:
    33 (0) : +(FLOAT, FLOAT)
    34 (1) : -(FLOAT, FLOAT)
    35 (2) : *(FLOAT, FLOAT)
    36 (3) : /(FLOAT, FLOAT)
    37 (4) : toFloat(INT)
*/

// Making the following functions inline should speed up mutation

inline struct Node randIntFunc(){
    const int intWeights[] = {1,1,1,1,1,4};
    const int intTotalWeight = 9;
    const int count = sizeof(intWeights)/sizeof(int);

    int v = rand() % intTotalWeight;
    int i;
    for(i=0; i<count; i++){
        if(v < intWeights[i])
            break;
        v -= intWeights[i];
    }

    struct Node result;
    result.type = 0;
    result.val = i+1;

    return result;
};

inline struct Node randFloatFunc(){
    const int floatWeights[] = {1,1,1,1,5};
    const int floatTotalWeight = 9;
    const int count = sizeof(floatWeights)/sizeof(int);

    int v = rand() % floatTotalWeight;
    int i;
    for(i=0; i<count; i++){
        if(v < floatWeights[i])
            break;
        v -= floatWeights[i];
    }

    struct Node result;
    result.type = 0;
    result.val = i+33;

    return result;
};

inline struct Node randFunc(enum NodeReturnType t){
    switch(t){
        case INT:
            return randIntFunc();
        case FLOAT:
            return randFloatFunc();
    };
}