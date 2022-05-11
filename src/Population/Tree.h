#pragma once

#include <vector>

#include "Node.h"

const int nRecords = 899;
const int numFloats = 8;

class Tree{
    std::vector<Node> nodes;

    int exec(int* data, int record);
};
