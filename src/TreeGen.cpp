#include "TreeGen.h"

#include <cmath>
#include <stack>
#include <tuple>

using namespace std;

struct Node getRandTerminal(){
    return Node{2,0};
}


vector<struct Node> generateFullTree(unsigned short maxDepth){
    // Prepare array in which tree will be built
    // Last node is a NULL
    int numNodes = pow(2, maxDepth + 1);
    vector<struct Node> tree(numNodes);

    // Populate tree
    int index = 0;
    int currDepth = 0;
    stack<tuple<int, enum NodeReturnType>> unfilled;
    enum NodeReturnType nextType = INT; //tree result must be INT

    while(true){
        if(currDepth < maxDepth - 1){
            // Funciton node
            tree[index] = randFunc(nextType);
            currDepth++;
            nextType = tree[index].getParam1();
            enum NodeReturnType type2 = tree[index].getParam2();
            // Not all functions take 2 parameters
            if(type2 != NONE)
                unfilled.push(make_tuple(currDepth, type2));
            index++;
        }
        else{
            // Terminal node
            tree[index] = randTerminal(nextType);
            index++;
            if(!unfilled.empty()){
                tuple<int, enum NodeReturnType> t = unfilled.top();
                unfilled.pop();
                currDepth = get<0>(t);
                nextType = get<1>(t);
            }
            else break;
        }
    }

    tree[index] = Node{0,0};

    tree.resize(index+1);
    return tree;
}
