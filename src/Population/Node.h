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
