#pragma once

#include <vector>

#include "Productions.h"
#include "Node.h"

const int nRecords = 899;
const int numFloats = 8;

class Tree{
public:
    bool mustRegenerate = true; //Need to derive nodes from codons

    std::vector<Node> nodes;
    std::vector<unsigned char> codons;

    int exec(int* data, int record);


    void randCodons(int len);
    void treeFromCodons();
};
