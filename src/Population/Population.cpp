#include "Population.h"

#include <cstring>
#include <tuple>

#include <Helpers.h>
#include <Parameters.h>
#include "TreeGen.h"

using namespace std;

Population::Population(){
    for(int i : OP_WEIGHTS){
        opTotalWeight += i;
    }
}

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
    int lastFullTournament = (popSize/TOURNAMENT_SIZE)*TOURNAMENT_SIZE;

    /*
    Need to select top n and bottom n*m performers
    Keep ordered list of best n so far: tuple<score, index>
    For 1st n records, insert into list (keep ordered)
    After that, compare with last in list(worst of the best) and work up from there

    */

    int baseIndex = 0;
    while(baseIndex < lastFullTournament){
        vector<tuple<float, int>> best;
        // Initialise best list with first elements from tournament
        for(int i=0; i<TOURNAMENT_WINNERS; i++){
            // evaluation order is according to order array, not linearly
            int fIndex = order[baseIndex + i];
            tuple<float,int> t = make_tuple(fitness[fIndex], fIndex);

            // Find index in list
            // List should be small, so a linear search is fine
            int j=0;
            for(tuple<float, int> b : best){
                if(get<0>(b) < get<0>(t))
                    j++;
                else break;
            }
            best.insert(best.begin() + j, t);
        }

        // Run through rest of tournament
        for(int i=TOURNAMENT_WINNERS; i<TOURNAMENT_SIZE; i++){
            int fIndex = order[baseIndex + i];

            // Find insert location, if any
            int j = TOURNAMENT_WINNERS;
            while(fitness[fIndex] < get<0>(best[j-1]) && j > 0)
                j--;

            if(j < TOURNAMENT_WINNERS){
                tuple<float,int> t = make_tuple(fitness[fIndex], fIndex);
                best.insert(best.begin()+j, t);
                best.resize(TOURNAMENT_WINNERS);
            }
        }

        // Now repeat for worst performers
        vector<tuple<float, int>> worst;
        int numLosers = TOURNAMENT_WINNERS*TOURNAMENT_MULTIPLICATION;
        for(int i=0; i<numLosers; i++){
            int fIndex = order[baseIndex + i];
            tuple<float,int> t = make_tuple(fitness[fIndex], fIndex);

            int j=0;
            for(tuple<float, int> b : worst){
                if(get<0>(b) >= get<0>(t))
                    j++;
                else break;
            }
            worst.insert(worst.begin() + j, t);
        }

        for(int i=numLosers; i<TOURNAMENT_SIZE; i++){
            int fIndex = order[baseIndex + i];

            int j = numLosers;
            while(fitness[fIndex] >= get<0>(worst[j-1]) && j > 0)
                j--;

            if(j < numLosers){
                tuple<float,int> t = make_tuple(fitness[fIndex], fIndex);
                worst.insert(worst.begin()+j, t);
                worst.resize(numLosers);
            }
        }

        // best and worst lists are now populated
        int w = 0;
        for(int i=0; i<TOURNAMENT_WINNERS; i++){
            int bIndex = get<1>(best[i]);
            for(int j=0; j<TOURNAMENT_MULTIPLICATION; j++){
                int wIndex = get<1>(worst[w]);
                w++;
                result.push_back(wIndex);
                trees[wIndex] = trees[bIndex];
            }
        }
        baseIndex += TOURNAMENT_SIZE;
    }
    return result;
}

tuple<int, int> randSubtree(const vector<Node>& tree, enum NodeReturnType type){
    if(type == NONE){
        int start = 1;
        if(tree.size() > 2)
            start = (rand() % (tree.size() - 2)) + 2;

        // Figure out how far subtree goes
        int end = start;
        int unresolved = tree[start].nParameters();
        while(unresolved > 0){
            end++;
            unresolved += (tree[end].nParameters() - 1);
        }

        return make_tuple(start, end);
    }

    int start=1;
    if(tree.size() > 2){
        start = (rand() % (tree.size() - 2)) + 2;
    }
    else{
        if(tree[start].getReturnType() != type)
            return make_tuple(-1, -1); //Can't recover if tree is too small
    }

    int s = start;
    bool dir = true;
    while(tree[start].getReturnType() != type){
        if(dir){
            start++;
            if(start == tree.size()){
                dir = false;
                start = s-1;
            }
        }
        else{
            start--;
            if(start == 0)
                return make_tuple(-1, -1); //Doesn't contain node of type
        }
    }

    int end = start;
    int unresolved = tree[start].nParameters();
    while(unresolved > 0){
        end++;
        unresolved += (tree[end].nParameters() - 1);
    }

    return make_tuple(start, end);
}

int mutate(vector<Node>& tree){
    int numNodes = -tree.size();

    tuple<int,int> subtree = randSubtree(tree);
    int start = get<0>(subtree);
    int end = get<1>(subtree);


    NodeReturnType type = tree[start].getReturnType();

    // Get new new subtree
    vector<Node> replacement = generateGrowTree(3, type);
    int removeLen = end - start + 1;
    int insertLen = replacement.size() - 1; //excludes starting NULL

    // This should be optimized effectively with memmove operations
    tree.erase(tree.begin()+start, tree.begin() + end + 1);
    tree.insert(tree.begin()+start, replacement.begin() + 1, replacement.end());

    return numNodes + tree.size();
}

int mutateIsEmpty(vector<Node>& tree){
    int numNodes = -tree.size();

    tuple<int,int> subtree = randSubtree(tree);
    int start = get<0>(subtree);
    int end = get<1>(subtree);


    NodeReturnType type = tree[start].getReturnType();

    int inIndex = randTerminal(type, false).val;

    // Get new new subtree
    vector<Node> replacement = isEmpty(type, inIndex);
    int removeLen = end - start + 1;
    int insertLen = replacement.size() - 1; //excludes starting NULL

    // This should be optimized effectively with memmove operations
    tree.erase(tree.begin()+start, tree.begin() + end + 1);
    tree.insert(tree.begin()+start, replacement.begin() + 1, replacement.end());

    return numNodes + tree.size();
}

void crossover(vector<Node>& lhs, vector<Node>& rhs){
    tuple<int,int> subtreeL, subtreeR;

    subtreeL = randSubtree(lhs);
    NodeReturnType type = lhs[get<0>(subtreeL)].getReturnType();

    subtreeR = randSubtree(rhs, type);

    if(get<0>(subtreeR) == -1){
        subtreeR = randSubtree(rhs);
        NodeReturnType type = rhs[get<0>(subtreeR)].getReturnType();

        subtreeL = randSubtree(lhs, type);

        //This should be unreachable since both trees have at least one INT node
        if(get<0>(subtreeL) == -1){
            return;
        }
    }
    // Can assume valid crossover points from here

    // A pretty standard swap, but with a messy syntax
    vector<Node> temp(lhs.begin() + get<0>(subtreeL), lhs.begin() + get<1>(subtreeL) + 1);
    lhs.erase(lhs.begin() + get<0>(subtreeL), lhs.begin() + get<1>(subtreeL) + 1);
    lhs.insert(lhs.begin() + get<0>(subtreeL), rhs.begin() + get<0>(subtreeR),
            rhs.begin() + get<1>(subtreeR) + 1);

    rhs.erase(rhs.begin() + get<0>(subtreeR),rhs.begin() + get<1>(subtreeR) + 1);
    rhs.insert(rhs.begin() + get<0>(subtreeR), temp.begin(), temp.end());

}


void Population::applyGenOps(std::vector<int> pool){
    // For each, select random genetic operator
    const int count = sizeof(OP_WEIGHTS)/sizeof(int);

    vector<int> crossoverPool;

    for(int t : pool){
        // Pick operator to apply
        int v = rand() % opTotalWeight;
        int i;
        for(i=0; i<count; i++){
            if(v < OP_WEIGHTS[i])
            break;
            v -= OP_WEIGHTS[i];
        }

        // Apply selected operator (i)
        switch (i) {
            case 0: //crossover
                crossoverPool.push_back(t);
                break;
            case 1: //mutation
                numNodes += mutate(trees[t]);
                break;
            case 2:
                numNodes += mutateIsEmpty(trees[t]);
                break;
        }
    }

    // Odd number ignored
    for(int i=0; i+1<crossoverPool.size(); i+= 2){
        crossover(trees[crossoverPool[i]], trees[crossoverPool[i+1]]);
    }

}

unsigned int Population::recalcNumNodes(){
    unsigned int result = 0;
    for(int i=0; i < trees.size(); i++){
        result += trees[i].size();
    }
    result -= trees.size(); //Exclude nulls (one per tree)
    numNodes = result;
    return result;
}
