#include "cpuExec.h"

#include <stack>
#include <climits>

#define castF(a) *(float*)(void*)(&a)
#define castI(a) *(int*)(void*)(&a)

using namespace std;

const int nRecords = 899;
const int numFloats = 8;

int execTree(Node* node, int* data, int r){
    stack<int> compStack;

    while(*node != Node{0,0}){
        // Go to 1st function node
        while(node->type != 0)
            node--;

        int left = INT_MAX, right = INT_MAX;

        // Get n parameters
        int nParams = node->nParameters();
        unsigned char filled = 0;

        // Check for terminals
        if(node[1].type != 0){
            if(node[1].type == 2) //int
                left = data[nRecords*(numFloats + node[1].val)+r];
            else //float
                left = data[nRecords*(node[1].val)+r];
            filled++;

            if(node[2].type != 0){
                if(node[2].type == 2) //int
                    right = data[nRecords*(numFloats + node[2].val)+r];
                else //float
                    right = data[nRecords*(node[2].val)+r];
                filled++;
            }
        }
        // If not enough terminals, take values from stack
        if(filled < nParams){
            if(filled == 0){
                left = compStack.top();
                compStack.pop();
                filled++;
            }
            if(filled < nParams){
                right = compStack.top();
                compStack.pop();
            }
        }

        // calc result, put on stack
        int result;
        if(!(node[0].val & 32)){
            switch(node[0].val){
                case 1: //+
                    result = left + right;
                    break;
                case 2: //-
                    result = left - right;
                    break;
                case 3: //*
                    result = left * right;
                    break;
                case 4: //divide
                    if(right == 0) right = 1;
                    result = left / right;
                    break;
                case 5: //mod
                    if(right < 1) right = 1;
                    result = left % right;
                    break;
                case 6: //round
                    result = (int) castF(left);
                    break;
            }
        }
        else{
            float fResult;
            switch(node[0].val){
                case 33: //+
                    fResult = castF(left) + castF(right);
                    break;
                case 34: //-
                    fResult = castF(left) - castF(right);
                    break;
                case 35: //*
                    fResult = castF(left) * castF(right);
                    break;
                case 36:{ //divide
                    float fRight = castF(right);
                    if(fRight == 0)
                        fRight = 1;
                    fResult = castF(left) / fRight;
                    break;
                }
                case 37:
                    fResult = left;
                    break;
            }
            result = castI(fResult);
        }

        compStack.push(result);
        node--;
    }
    // the only value on the stack is the answer

    return compStack.top();
}
