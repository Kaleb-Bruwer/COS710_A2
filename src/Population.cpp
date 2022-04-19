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
            startIndexes.push_back(filledSize);
            filledSize += result.size();
            treesVec.insert(treesVec.end(), result.begin(), result.end());
        }
    }

    trees = new Node[filledSize];
    memcpy(trees, &treesVec[0], filledSize * sizeof(Node));
}
