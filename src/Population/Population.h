#pragma once

#include <vector>

#include "Node.h"
#include "GPUNode.h"

class Population{
public:
    Node* trees = 0;
    GPUNode* gpuTrees = 0;
    std::vector<unsigned int> startIndexes;
    unsigned int numNodes = 0;

    Population(){};
    ~Population();

    void rampedFull(int popSize, int maxDepth);

    void makeGPUTrees();

};
