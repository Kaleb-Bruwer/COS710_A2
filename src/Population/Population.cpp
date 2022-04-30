#include "Population.h"

#include <cstring>

#include <Helpers.h>
#include "TreeGen.h"

using namespace std;

Population::~Population(){
    // if(trees)
    //     delete [] trees;
}

void Population::rampedFull(int p, int maxDepth){
    int index = trees.size(); //If there already are trees, this won't be 0

    int numDone = 0;
    for(int i=2; i<=maxDepth; i++){
        int nextStop = ((i-1)*p)/(double)(maxDepth - 1);
        for(; numDone < nextStop; numDone++){
            trees.push_back(generateFullTree(i));
            numNodes += trees[index].size() - 1; //Tree includes one null
            index++;
        }
    }

    // nextStop may sometimes be broken by a floating point error,
    // this code is to make sure the full amount of trees is always generated
    for(;numDone < p; numDone++){
        trees.push_back(generateFullTree(maxDepth));
        numNodes += trees[index].size() - 1;
        index++;
    }

    popSize = index;
}

void Population::rampedGrow(int p, int maxDepth){
    int index = trees.size(); //If there already are trees, this won't be 0

    int numDone = 0;
    for(int i=2; i<=maxDepth; i++){
        int nextStop = ((i-1)*p)/(double)(maxDepth - 1);
        for(; numDone < nextStop; numDone++){
            trees.push_back(generateGrowTree(i));
            numNodes += trees[index].size() - 1; //Tree includes one null
            index++;
        }
    }

    // nextStop may sometimes be broken by a floating point error,
    // this is to make sure the full amount of trees is always generated
    for(;numDone < p; numDone++){
        trees.push_back(generateGrowTree(maxDepth));
        numNodes += trees[index].size() - 1;
        index++;
    }
    popSize = index;
}

void Population::rampedHalfHalf(int p, int maxDepth){
    int half = p/2;
    rampedFull(half, maxDepth);
    rampedGrow(p-half, maxDepth); //Avoids rounding errors
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

vector<int> Population::tournamentSelection(float* fitness){
    vector<int> result;

    // Randomize evaluation order
    int popSize = trees.size();

    int* order = new int[popSize];
    for(int i=0; i<popSize; i++){
        order[i] = i;
    }
    shuffle(order, popSize);


    // Divide into tournaments based on above order
    // Incomplete tournament at end gets ignored
    int lastFullTournament = (popSize/tournamentSize)*tournamentSize;

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

int mutate(std::vector<Node>& tree){
    int numNodes = -tree.size();
    // Can't be the null or root node
    int start = 1;
    if(tree.size() > 2)
        start = (rand() % (tree.size() - 2)) + 2;

    NodeReturnType type;
    switch(tree[start].type){
        case 0:
            type = (tree[start].val & 32) ? FLOAT : INT;
            break;
        case 1:
        case 2:
            type = INT;
            break;
        case 3:
            type = FLOAT;
            break;
    }

    // Figure out how far subtree goes
    int end = start;
    int unresolved = tree[start].nParameters();
    while(unresolved > 0){
        end++;
        unresolved += (tree[end].nParameters() - 1);
    }

    // Get new new subtree
    vector<Node> replacement = generateGrowTree(3, type);
    int removeLen = end - start + 1;
    int insertLen = replacement.size() - 1; //excludes starting NULL

    // This should be optimized effectively with memmove operations
    tree.erase(tree.begin()+start, tree.begin() + end + 1);
    tree.insert(tree.begin()+start, replacement.begin() + 1, replacement.end());

    return numNodes + tree.size();
}


void Population::applyGenOps(std::vector<int> pool){
    // For each, select random genetic operator

    // [Crossover, Mutation]
    const int opWeights[] = {7,1};
    const int opTotalWeight = 8;
    const int count = sizeof(opWeights)/sizeof(int);



    for(int t : pool){
        // Pick operator to apply
        int v = rand() % opTotalWeight;
        int i;
        for(i=0; i<count; i++){
            if(v < opWeights[i])
            break;
            v -= opWeights[i];
        }

        // Apply selected operator (i)
        switch (i) {
            case 0: //crossover
                break;
            case 1: //mutation
                numNodes += mutate(trees[t]);
                break;
        }
    }
}
