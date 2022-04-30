#pragma once

#include <vector>

#include "Node.h"
#include "GPUNode.h"

class Population{
public:
    // Node* trees = 0;
    std::vector<std::vector<Node>> trees;

    GPUNode* gpuTrees = 0;
    // std::vector<unsigned int> startIndexes;
    unsigned int numNodes = 0;

    int tournamentSize = 5;

    Population(){};
    ~Population();

    void rampedFull(int popSize, int maxDepth);
    void rampedGrow(int popSize, int maxDepth);
    void rampedHalfHalf(int popSize, int maxDepth);
    void makeGPUTrees(){};

    // clones best performers over worst-performers
    // returns indexes of these clones, which is the pool for genetic operators
    std::vector<int> tournamentSelection(std::vector<float> fitness);

    void applyGenOps(std::vector<int> pool);

};
