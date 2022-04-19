#pragma once

#include "Node.h"

/*
bottom: last node of tree, func works back from it until NULL reached
data: pointer to data in gpu format
r: record index
*/
int execTree(Node* bottom, int* data, int r);
