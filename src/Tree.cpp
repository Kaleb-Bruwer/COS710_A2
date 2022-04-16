#include "Tree.h"

#include <cmath>
#include <stack>

using namespace std;

struct Node getRandFunc(){
    return Node{0,0};
}

struct Node getRandTerminal(){
    return Node{2,0};
}


void generateFullTree(unsigned short maxDepth){
    // Prepare array in which tree will be built
    int numNodes = pow(2, maxDepth + 1) - 1;
    Node* tree = 0;

    bool stackAllocation = (numNodes < 256);
    if(stackAllocation)
        tree = (Node*) alloca(numNodes);
    else
        tree = new Node[numNodes];

    // Populate tree
    int index = 0;
    int currDepth = 0;
    stack<int> unfilled;

    tree[index] = getRandFunc();
    currDepth++;
    unfilled.push(currDepth);
    index++;

    bool justPopped = false;
    while(true){
        if(currDepth < maxDepth - 1){
            // Another funciton node
            tree[index] = getRandFunc();
            currDepth++;
            unfilled.push(currDepth);
            index++;
        }
        else{
            // A terminal node
            tree[index] = getRandTerminal();
            tree[index+1] = getRandTerminal();
            index += 2;
            if(!unfilled.empty()){
                currDepth = unfilled.top();
                unfilled.pop();
            }
            else break;
        }
    }


    if(!stackAllocation)
        delete [] tree;
}
