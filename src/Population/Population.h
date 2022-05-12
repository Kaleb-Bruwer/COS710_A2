#pragma once

#include <vector>
#include <tuple>

#include "Node.h"
#include "GPUNode.h"
#include "Tree.h"

class Population{
private:
    int opTotalWeight = 0;
public:
    int numNodes;

    std::vector<Tree> trees;

    Population();
    ~Population(){};

    void initPop(int popSize); //Generate initial population

    // clones best performers over worst-performers
    // returns indexes of these clones, which is the pool for genetic operators
    // Input aArray must be of size popSize
    std::vector<int> tournamentSelection(float* fitness);

    void applyGenOps(std::vector<int> pool);

    void recalcNumNodes();
};

void mutate(Tree &tree);
void crossover(Tree &t1, Tree &t2);
