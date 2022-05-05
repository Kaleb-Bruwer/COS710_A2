#include "TreeGen.h"

#include <cmath>
#include <stack>
#include <tuple>

using namespace std;

struct Node getRandTerminal(){
    return Node{2,0};
}

vector<struct Node> isEmpty(enum NodeReturnType inType, int inIndex){
    vector<struct Node> tree(1, Node{0,0});

    Node input{2,inIndex};
    if(! inType==INT)
        input.type = 3;

    if(inType == INT){
        tree.push_back(Node{0, 7});//IF
        tree.push_back(Node{0,17}); //==
    }
    else{
        tree.push_back(Node{0, 38});//IF
        tree.push_back(Node{0, 18});//==
    }

    tree.push_back(input);

    Node neg9 = Node{1,0};
    neg9.setConst(-9);
    tree.push_back(neg9);

    vector<struct Node> subtree = generateGrowTree(3, inType);
    tree.insert(tree.end(), subtree.begin()+1, subtree.end());

    tree.push_back(input);

    return tree;
}


vector<struct Node> generateFullTree(unsigned short maxDepth){
    // Prepare array in which tree will be built
    // First node is a NULL (last encountered during execution)

    vector<struct Node> tree(1);

    // Populate tree
    tree[0] = Node{0,0};
    int index = 1;
    int currDepth = 0;
    stack<tuple<int, enum NodeReturnType>> unfilled;
    enum NodeReturnType nextType = INT; //tree result must be INT

    while(true){
        if(currDepth < maxDepth - 1){
            // Funciton node
            tree.push_back(randFunc(nextType));
            currDepth++;
            nextType = tree[index].getParamType(0);
            enum NodeReturnType type2 = tree[index].getParamType(1);
            // Not all functions take 2 parameters
            if(type2 != NONE){
                // IF has 3 parameters
                enum NodeReturnType type3 = tree[index].getParamType(2);
                if(type3 != NONE){
                    unfilled.push(make_tuple(currDepth, type3));
                }
                unfilled.push(make_tuple(currDepth, type2));
            }
            index++;
        }
        else{
            // Terminal node
            tree.push_back(randTerminal(nextType));
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
    return tree;
}

// nextType's default argument is INT, it is changeable for the sake of generating
// mutations
vector<struct Node> generateGrowTree(unsigned short maxDepth, enum NodeReturnType nextType){
    // Prepare array in which tree will be built
    // First node is a NULL (last encountered during execution)

    // numNodes initialized to max possible size, will be resized at end
    vector<struct Node> tree(1);

    // Populate tree
    tree[0] = Node{0,0};
    int index = 1;
    int currDepth = 0;
    stack<tuple<int, enum NodeReturnType>> unfilled;

    while(true){
        if(currDepth < maxDepth - 1 && pickFunc()){
            // Funciton node
            tree.push_back(randFunc(nextType));
            currDepth++;
            nextType = tree[index].getParamType(0);
            enum NodeReturnType type2 = tree[index].getParamType(1);
            // Not all functions take 2 parameters
            if(type2 != NONE){
                // IF has 3 parameters
                enum NodeReturnType type3 = tree[index].getParamType(2);
                if(type3 != NONE){
                    unfilled.push(make_tuple(currDepth, type3));
                }
                unfilled.push(make_tuple(currDepth, type2));
            }
            index++;
        }
        else{
            // Terminal node
            tree.push_back(randTerminal(nextType));
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
    return tree;
}
