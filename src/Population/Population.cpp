#include "Population.h"

#include <cstring>
#include <tuple>

#include <Helpers.h>
#include <Parameters.h>

using namespace std;

Population::Population(){
    for(int i : OP_WEIGHTS){
        opTotalWeight += i;
    }
}

void Population::initPop(int p){
    trees.resize(p);
    for(int i=0; i<p; i++){
        trees[i].randCodons();
        trees[i].treeFromCodons();
    }
    recalcNumNodes();
}

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

void mutate(Tree &tree){
    int index = rand() % tree.codons.size();
    tree.codons[index] = rand();
    tree.mustRegenerate = true;
}

void crossover(Tree &t1, Tree &t2){
    // Wrap-around crossover
    // Assumes codons are of the same size
    t1.mustRegenerate = true;
    t2.mustRegenerate = true;

    int size = t1.codons.size();

    int len = rand() % size;
    int start1 = rand() % size;
    int start2 = rand() % size;

    // Having two temps makes it easier to deal with wrap-around

    vector<unsigned char> temp1(len, 0);
    vector<unsigned char> temp2(len, 0);
    int end1 = start1 + len;
    int end2 = start2 + len;

    bool wrap1 = false;
    bool wrap2 = false;

    // NOTE: selection excludes end index
    if(end1 > size){
        memcpy(&temp1[0], &t1.codons[start1], (size-start1) * sizeof(unsigned char));

        wrap1 = true;
        memcpy(&temp1[size-start1], &t1.codons[0], (len - (size - start1)) * sizeof(unsigned char));
    }
    else{
        memcpy(&temp1[0], &t1.codons[start1], len * sizeof(unsigned char));
    }


    if(end2 > size){
        wrap2 = true;
        memcpy(&temp2[0], &t2.codons[start2], (size-start2) * sizeof(unsigned char));
        memcpy(&temp2[size-start2], &t2.codons[0], (len - (size - start2)) * sizeof(unsigned char));
    }
    else{
        memcpy(&temp2[0], &t2.codons[start2], len * sizeof(unsigned char));
    }


    // Now put temps back in other trees
    if(wrap1){
        memcpy(&t1.codons[start1], &temp2[0], (size - start1) * sizeof(unsigned char));
        memcpy(&t1.codons[0], &temp2[size-start1], (len -(size - start1)) * sizeof(unsigned char));
    }
    else{
        memcpy(&t1.codons[start1], &temp2[0], len * sizeof(unsigned char));
    }

    if(wrap2){
        memcpy(&t2.codons[start2], &temp1[0], (size - start2) * sizeof(unsigned char));
        memcpy(&t2.codons[0], &temp1[size-start2], (len -(size - start2)) * sizeof(unsigned char));
    }
    else{
        memcpy(&t2.codons[start2], &temp1[0], len * sizeof(unsigned char));
    }
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
                mutate(trees[t]);
                break;
        }
    }

    // Odd number ignored
    for(int i=0; i+1<crossoverPool.size(); i+= 2){
        crossover(trees[crossoverPool[i]], trees[crossoverPool[i+1]]);
    }

}

void Population::recalcNumNodes(){
    numNodes = 0;
    for(int i=0; i<trees.size(); i++){
        numNodes+= trees[i].nodes.size();
    }
    numNodes -= trees.size(); //Excludes initial 0's
}
