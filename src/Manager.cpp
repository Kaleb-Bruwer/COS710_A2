#include "Manager.h"

#include "cpuExec.h"
#include <iostream>

using namespace std;

#define castF(a) *(float*)(void*)(&a)

void Manager::initialize(int p){
    // Load data
    dataLoader.addFromFile("../data/cleveland.data", 282);
    dataLoader.addFromFile("../data/hungarian.data", 294);
    dataLoader.addFromFile("../data/long-beach-va.data", 200);
    dataLoader.addFromFile("../data/switzerland.data", 123);
    dataLoader.makeGPUFormat();
    dataLoader.dropRawData();

    // Generate initial population
    popSize = p;
    population.rampedFull(p, 15);
    fitness = new float[p];

    // Initialise shader
    compShader.initialize();
    compShader.loadData(dataLoader.getGPUData(), dataLoader.sizeofGPUData());

}

void Manager::printInfo(){
    cout << "num Trees: " << popSize << endl;
    cout << "num Training case: " << trainSize << endl;
    cout << "total nodes (excl nulls): " << population.startIndexes[popSize-1] - popSize << endl;

    // int* data = (int*)dataLoader.getGPUData();
    // for(int i=0; i<trainSize; i++){
    //     cout << data[899*(8+46) + i] << "  ";
    // }
    // cout << endl;
}


void Manager::runCPU(){
    int* data = (int*)dataLoader.getGPUData();

    int* results = new int[numInputs];
    for(int i=0; i<popSize; i++){
        Node* bottom = &population.trees[population.startIndexes[i]];

        for(int j=0; j<numInputs; j++){
            results[j] = execTree(bottom, data, j);
        }

        int targetBaseIndex = 899*(8+46);
        int correctTrain = 0;
        int errorTrain = 0;
        for(int j=0; j<trainSize; j++){
            if(data[targetBaseIndex+j] == results[j])
                correctTrain++;
            errorTrain += abs(data[targetBaseIndex+j] - results[j]);
        }

        int correctTest = 0;
        int errorTest = 0;
        for(int j=0; j<trainSize; j++){
            if(data[targetBaseIndex+j] == results[j])
                correctTest++;
            errorTest += abs(data[targetBaseIndex+j] - results[j]);
        }
        fitness[i] = errorTrain;
    }
    delete [] results;
}
