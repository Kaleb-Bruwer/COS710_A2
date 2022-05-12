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
    std::vector<Tree> trees;
    // std::vector<std::vector<Node>> trees;
    // int popSize = 0;


    Population();
    ~Population(){};

    void initPop(int popSize); //Generate initial population

    // clones best performers over worst-performers
    // returns indexes of these clones, which is the pool for genetic operators
    // Input aArray must be of size popSize
    std::vector<int> tournamentSelection(float* fitness);

    void applyGenOps(std::vector<int> pool);
};

void mutate(Tree &tree);
void crossover(Tree &t1, Tree &t2);
