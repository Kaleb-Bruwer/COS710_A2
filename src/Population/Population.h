#pragma once

#include <vector>
#include <tuple>

#include "Node.h"
#include "GPUNode.h"

class Population{
public:
    // Node* trees = 0;
    std::vector<std::vector<Node>> trees;
    int popSize = 0;

    GPUNode* gpuTrees = 0;
    // std::vector<unsigned int> startIndexes;
    unsigned int numNodes = 0;

    int tournamentSize = 2;

    Population(){};
    ~Population();

    void rampedFull(int popSize, int maxDepth);
    void rampedGrow(int popSize, int maxDepth);
    void rampedHalfHalf(int popSize, int maxDepth);
    void makeGPUTrees(){};

    // clones best performers over worst-performers
    // returns indexes of these clones, which is the pool for genetic operators
    // Input aArray must be of size popSize
    std::vector<int> tournamentSelection(float* fitness);

    void applyGenOps(std::vector<int> pool);

};

std::tuple<int, int> randSubtree(const std::vector<Node>& tree, enum NodeReturnType type = NONE);

// Returns change to numNodes
int mutate(std::vector<Node>& tree);
int mutateIsEmpty(std::vector<Node>& tree);
void crossover(std::vector<Node>& lhs, std::vector<Node>& rhs);
