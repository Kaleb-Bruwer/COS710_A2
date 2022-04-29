#include "Population.h"

#include <cstring>

#include "TreeGen.h"

using namespace std;

Population::~Population(){
    // if(trees)
    //     delete [] trees;
}

// This momentarily uses twice as much memory as it needs, but it shouldn't be
// too hard to fix if that actually becomes a problem
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

// void Population::makeGPUTrees(){
//     if(!trees)
//         return;
//
//     gpuTrees = new GPUNode[numNodes];
//     for(int i=0; i<numNodes; i++){
//         gpuTrees[i] = GPUNode{trees[i].type, trees[i].val};
//     }
// }

vector<int> tournamentSelection(vector<float> fitness){
    // Randomize evaluation order


    // Divide into tournaments based on above order


    // Evaluate each tournament, clone winners over losers
}
