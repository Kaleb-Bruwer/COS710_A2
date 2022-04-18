#pragma once

#include "Node.h"

/*
A tree is a sequence of Nodes, where each takes only one byte and can be
eather an internal or terminal node
*/

struct Node getRandTerminal();

void generateFullTree(unsigned short maxDepth);
