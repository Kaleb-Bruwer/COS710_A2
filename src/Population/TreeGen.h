#pragma once

#include "Node.h"

#include <vector>

/*
A tree is a sequence of Nodes, where each takes only one byte and can be
either an internal or terminal node
*/

// Fraction out of 1000, odds of making a new node instead of a terminal
inline bool pickFunc(){
    return ((rand() % 1000) <= GROW_RATE);
};


struct Node getRandTerminal();

std::vector<struct Node> isEmpty(enum NodeReturnType inType, int inIndex);

std::vector<struct Node> generateFullTree(unsigned short maxDepth);
std::vector<struct Node> generateGrowTree(unsigned short maxDepth, enum NodeReturnType type = INT);
