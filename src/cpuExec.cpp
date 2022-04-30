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

    for(;*node != Node{0,0}; node--){

        switch(node->type){
            case 0:{ //function
                int result;
                int left, right;
                // All functions have at least one input
                left = compStack.top();
                compStack.pop();
                if(node->nParameters() == 2){
                    right = compStack.top();
                    compStack.pop();
                }

                if(!(node[0].val & 32)){ //checks if it's an int or float function
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
                break;
            }

            case 1: //const
                compStack.push(node->val);
                break;
            case 2: //INT
                compStack.push(data[nRecords*(numFloats + node->val)+r]);
                break;

            case 3: //FLOAT
                compStack.push(data[nRecords*(node->val)+r]);
                break;
        }//switch
    }//for

    // the only value still on the stack is the answer
    return compStack.top();
}
