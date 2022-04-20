#pragma once

#include <vector>

#include "Node.h"

class Population{
public:
    Node* trees = 0;
    std::vector<int> startIndexes;

    Population(){};
    ~Population();

    void rampedFull(int popSize, int maxDepth);

};
