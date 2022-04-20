#include "Population.h"

#include <cstring>

#include "TreeGen.h"

using namespace std;

Population::~Population(){
    if(trees)
        delete [] trees;
}

// This momentarily uses twice as much memory as it needs, but it shouldn't be
// too hard to fix if that actually becomes a problem
void Population::rampedFull(int popSize, int maxDepth){
    unsigned int filledSize = 0;
    vector<Node> treesVec; //consecutively stored

    int numDone = 0;
    for(int i=2; i<=maxDepth; i++){
        int nextStop = (i-1)*(popSize/(double)(maxDepth - 1));
        for(; numDone < nextStop; numDone++){
            vector<Node> result = generateFullTree(i);
            filledSize += result.size();
            startIndexes.push_back(filledSize-1);
            treesVec.insert(treesVec.end(), result.begin(), result.end());
        }
    }

    trees = new Node[filledSize];
    numNodes = filledSize;
    memcpy(&trees[0], &treesVec[0], filledSize * sizeof(Node));
}

void Population::makeGPUTrees(){
    if(!trees)
        return;

    gpuTrees = new GPUNode[numNodes];
    for(int i=0; i<numNodes; i++){
        gpuTrees[i] = GPUNode{trees[i].type, trees[i].val};
    }
}
