#pragma once

#include "Node.h"

#include <vector>

/*
A tree is a sequence of Nodes, where each takes only one byte and can be
either an internal or terminal node
*/

struct Node getRandTerminal();

std::vector<struct Node> generateFullTree(unsigned short maxDepth);
