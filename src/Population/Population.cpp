#include "Population.h"

#include <cstring>

#include <Helpers.h>
#include "TreeGen.h"

using namespace std;

Population::~Population(){
    // if(trees)
    //     delete [] trees;
}

void Population::rampedFull(int popSize, int maxDepth){
    unsigned int filledSize = 0;
    vector<Node> treesVec; //consecutively stored

    trees.resize(popSize);

    int numDone = 0;
    for(int i=2; i<=maxDepth; i++){
        int nextStop = ((i-1)*popSize)/(double)(maxDepth - 1);
        for(; numDone < nextStop; numDone++){
            trees[numDone] = generateFullTree(i);
            numNodes+= trees[numDone].size() - 1; //Tree includes one null
        }
    }

    // nextStop may sometimes be broken by a floating point error,
    // this is to make sure the full amount of trees is always generated
    for(;numDone < popSize; numDone++){
        trees[numDone] = generateFullTree(maxDepth);
        numNodes+= trees[numDone].size() - 1;
    }
}

void Population::rampedGrow(int popSize, int maxDepth){
    unsigned int filledSize = 0;
    vector<Node> treesVec; //consecutively stored

    trees.resize(popSize);

    int numDone = 0;
    for(int i=2; i<=maxDepth; i++){
        int nextStop = ((i-1)*popSize)/(double)(maxDepth - 1);
        for(; numDone < nextStop; numDone++){
            trees[numDone] = generateGrowTree(i);
            numNodes+= trees[numDone].size() - 1; //Tree includes one null
        }
    }

    // nextStop may sometimes be broken by a floating point error,
    // this is to make sure the full amount of trees is always generated
    for(;numDone < popSize; numDone++){
        trees[numDone] = generateGrowTree(maxDepth);
        numNodes+= trees[numDone].size() - 1;
    }
}

// void Population::makeGPUTrees(){
//     if(!trees)
//         return;
//
//     gpuTrees = new GPUNode[numNodes];
//     for(int i=0; i<numNodes; i++){
//         gpuTrees[i] = GPUNode{trees[i].type, trees[i].val};
//     }
// }

vector<int> Population::tournamentSelection(vector<float> fitness){
    vector<int> result;

    // Randomize evaluation order
    int numTrees = trees.size();

    int* order = new int[numTrees];
    for(int i=0; i<numTrees; i++){
        order[i] = i;
    }
    shuffle(order, numTrees);


    // Divide into tournaments based on above order
    // Incomplete tournament at end gets ignored
    int lastFullTournament = (numTrees/tournamentSize)*tournamentSize;

    int index = 0;
    while(index < lastFullTournament){
        float best = fitness[index];
        float worst = fitness[index];
        int bestIndex = index;
        int worstIndex = index;
        index++;


        for(int j=1; j<tournamentSize; j++){
            if(best > fitness[index]){
                best = fitness[index];
                bestIndex = index;
            }
            if(worst < fitness[index]){
                worst = fitness[index];
                worstIndex = index;
            }
            index++;
        }

        // Replace losers with clones of winners
        trees[worstIndex] = trees[bestIndex];
        result.push_back(worstIndex);

    }

    return result;
}
