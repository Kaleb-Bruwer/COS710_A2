#pragma once

#include <cstdlib>
#include <iostream>

#include <Parameters.h>

// Not to be confused with Node.type
enum NodeReturnType{
    INT,
    FLOAT,
    BOOL,
    NONE //returned by getParam if nothing is taken at that position
};

class Node{
public:
    //00: function, 01: const, 10: int input, 11: float input
    unsigned char type :2;
    //either input index or function number
    unsigned char val  :6;

    enum NodeReturnType getParamType(int);

    enum NodeReturnType getParam1();
    enum NodeReturnType getParam2();

    friend std::ostream& operator<<(std::ostream& out, const Node &n);
    friend bool operator==(const Node &lhs, const Node &rhs);
    friend bool operator!=(const Node &lhs, const Node &rhs);

    unsigned char fullVal(){
        return (type << 6) & val;
    };

    int getConst(){
        if(val & 32)
            return val - 64;
        return val;
    };
    void setConst(int v){ //Only works vor v in [-32,31]
        if(v < 0)
            v+= 64;
        val = v & 0x3f;
    };

    unsigned int nParameters() const{
        if(type != 0)
            return 0;
        if(val == 6 || val == 37)
            return 1;
        if(val == 7 || val == 38)
            return 3;
        return 2;
    };

    enum NodeReturnType getReturnType() const{
        switch(type){
            case 0:
                // if(val & 16) return BOOL;
                return (val & 32) ? FLOAT : INT;
            case 1:
            case 2:
                return INT;
            case 3:
                return FLOAT;
        }
    }

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
    07 (6) : IF(INT, INT, INT)
BOOL:
    17 (0) : ==(INT, INT)
    18 (1) : ==(FLOAT, FLOAT)
FLOAT:
    33 (0) : +(FLOAT, FLOAT)
    34 (1) : -(FLOAT, FLOAT)
    35 (2) : *(FLOAT, FLOAT)
    36 (3) : /(FLOAT, FLOAT)
    37 (4) : toFloat(INT)
    38 (5) : IF(INT, FLOAT, FLOAT)
*/

// Making the following functions inline should speed up mutation

inline struct Node randIntFunc(){
    // I'm hoping the sum of a const array is optimized out, it should be
    int intTotalWeight = 0;
    for(int i : INT_WEIGHTS)
        intTotalWeight+= i;
    const int count = sizeof(INT_WEIGHTS)/sizeof(int);

    int v = rand() % intTotalWeight;
    int i;
    for(i=0; i<count; i++){
        if(v < INT_WEIGHTS[i])
            break;
        v -= INT_WEIGHTS[i];
    }

    struct Node result;
    result.type = 0;
    result.val = i+1;

    return result;
};

inline struct Node randFloatFunc(){
    // I'm hoping the sum of a const array is optimized out, it should be
    int floatTotalWeight = 0;
    for(int i : FLOAT_WEIGHTS)
        floatTotalWeight+= i;
    const int count = sizeof(FLOAT_WEIGHTS)/sizeof(int);

    int v = rand() % floatTotalWeight;
    int i;
    for(i=0; i<count; i++){
        if(v < FLOAT_WEIGHTS[i])
            break;
        v -= FLOAT_WEIGHTS[i];
    }

    struct Node result;
    result.type = 0;
    result.val = i+33;

    return result;
};

inline struct Node randConst(){
    return Node{1,rand()};
};

inline struct Node randTerminal(enum NodeReturnType t, bool allowConst = true){
    const int numFloats = 8;
    const int numInts = 61; //excludes target
    const int targetPos = 46;

    Node result{0,0};

    switch(t){
        case INT:
            if(allowConst && (rand() % 1000 < CONST_CHANCE))
                return randConst();
            result.type = 2;
            result.val = rand() % numInts;
            if(result.val >= targetPos)
                result.val++;
            break;
        case FLOAT:
            result.type = 3;
            result.val = rand() % numFloats;
            break;
    }
    return result;
};

inline struct Node randFunc(enum NodeReturnType t){
    switch(t){
        case INT:
            return randIntFunc();
        case FLOAT:
            return randFloatFunc();
    };
};
