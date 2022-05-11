#include "Tree.h"

#include <stack>
#include <climits>

using namespace std;

#define castF(a) *(float*)(void*)(&a)
#define castI(a) *(int*)(void*)(&a)

int Tree::exec(int* data, int r){
    stack<int> compStack;
    Node* node = &nodes[nodes.size() - 1];

    for(;*node != Node{0,0}; node--){

        switch(node->type){
            case 0:{ //function
                int result;
                int left, right, param3;
                // All functions have at least one input
                left = compStack.top();
                compStack.pop();
                int nParams = node->nParameters();

                if(nParams > 1){
                    right = compStack.top();
                    compStack.pop();

                    if(nParams > 2){
                        param3 = compStack.top();
                        compStack.pop();
                    }
                }

                if(!(node[0].val & 32)){ //checks if it's an int or float function
                    switch(node[0].val){
                        // INT functions
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
                        case 7: //IF
                            result = left ? right : param3;
                            break;
                        // BOOL functions
                        case 17:
                            result = (left == right);
                            break;
                        case 18:
                            result = (castF(left) == castF(right));
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
                        case 38: //IF
                            fResult = left ? castF(right) : castF(param3);
                            break;
                    }
                    result = castI(fResult);
                }

                compStack.push(result);
                break;
            }

            case 1:{ //const
                compStack.push(node->getConst());
                break;
            }
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

void Tree::treeFromCodons(){
    // TODO: depth info may be necessary
    ProductionTable* pt = ProductionTable::getInstance();


    nodes.clear();

    stack<GenerateNode> prodStack;
    prodStack.push(GenerateNode(RET_INT));

    int c = 0; //codon index

    while(!prodStack.empty()){
        GenerateNode curr = prodStack.top();
        prodStack.pop();

        if(curr.type){//Node
            nodes.push_back(curr.node);
        }
        else{//Production
            vector<GenerateNode> result = pt->getProduction(curr.production, codons[c]);
            c++;
            if(c == codons.size())
                c = 0;

            // First nodes go directly to the tree
            int start;
            for(start=0; start < result.size(); start++){
                if(curr.type)
                    nodes.push_back(result[start].node);
                else
                    break;
            }

            // Rest goes to the prodStack
            for(int i=result.size() - 1; i >= start; i--){
                prodStack.push(result[i]);
            }
        }
    }
    mustRegenerate = false;
}


void Tree::randCodons(int len){
    codons.resize(len);
    for(int i=0; i<len; i++){
        codons[i] = rand();
    }
}
