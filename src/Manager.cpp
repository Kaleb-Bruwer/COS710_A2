#include "Manager.h"

#include "cpuExec.h"
#include <iostream>
#include <thread>

using namespace std;

#define castF(a) *(float*)(void*)(&a)

Manager::~Manager(){
    if(accuracy)
        delete [] accuracy;
    if(fitness)
        delete [] fitness;
}


void Manager::initialize(int p, int maxDepth){
    // Load data
    dataLoader.addFromFile("../data/cleveland.data", 282);
    dataLoader.addFromFile("../data/hungarian.data", 294);
    dataLoader.addFromFile("../data/long-beach-va.data", 200);
    dataLoader.addFromFile("../data/switzerland.data", 123);
    dataLoader.makeGPUFormat();
    dataLoader.dropRawData();

    // Generate initial population
    popSize = p;
    population.rampedHalfHalf(p, maxDepth);
    population.makeGPUTrees();
    fitness = new float[p];
    accuracy = new float[p];

    // // Initialise shader
    // compShader.initialize();
    // compShader.loadData(dataLoader.getGPUData(), dataLoader.sizeofGPUData());
    // compShader.loadTrees(population.gpuTrees, population.numNodes, population.startIndexes);
    //
    // GLint t[3];
    // glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_SIZE, t);
    // cout << "GL_MAX_COMPUTE_WORK_GROUP_SIZE: " << t[0] << " " << t[1] << " " << t[2] << endl;
    //
    // glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_COUNT, t);
    // cout << "GL_MAX_COMPUTE_WORK_GROUP_COUNT: " << t[0] << " " << t[1] << " " << t[2] << endl;
    //
    // glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, t);
    // cout << "GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS: " << t[0] << " " << t[1] << " " << t[2] << endl;
}

void Manager::printInfo(){
    cout << "num Trees: " << popSize << endl;
    cout << "num Training cases: " << trainSize << endl;
    cout << "num Total cases: " << numInputs << endl;
    cout << "total nodes (excl nulls): " << population.numNodes << endl;

    // int* data = (int*)dataLoader.getGPUData();
    // for(int i=0; i<trainSize; i++){
    //     cout << data[899*(8+46) + i] << "  ";
    // }
    // cout << endl;
}

void Manager::printGenerationStats(int genNum){
    float avgAcc = 0;
    float avgFit = 0;

    for(int i=0; i<popSize; i++){
        avgAcc += accuracy[i];
        avgFit += fitness[i];
    }
    avgAcc /= popSize;
    avgFit /= popSize;

    float bestAcc = 0;
    for(int i=0; i<popSize; i++){
        if(accuracy[i] > bestAcc)
            bestAcc = accuracy[i];
    }

    cout << "Gen " << genNum << ": ";
    cout << "Accuracy (avg, best): (" << avgAcc << ", " << bestAcc << "), avg fitness: " << avgFit << endl;
    cout << "Num nodes: " << population.numNodes << endl;

}

void Manager::runCPUThread(int* data, int start, int end){
    int* results = new int[numInputs];

    for(int i=start; i<end; i++){
        // Node* bottom = &population.trees[population.startIndexes[i]];
        int treeSize = population.trees[i].size();
        Node* bottom = &population.trees[i][treeSize-1];

        for(int j=0; j<numInputs; j++){
            results[j] = execTree(bottom, data, j);
        }

        int targetBaseIndex = 899*(8+46);
        int correctTrain = 0;
        float errorTrain = 0;
        for(int j=0; j<trainSize; j++){
            if(data[targetBaseIndex+j] == results[j])
                correctTrain++;
            errorTrain += pow(abs(data[targetBaseIndex+j] - results[j]),2);
        }

        int correctTest = 0;
        float errorTest = 0;
        for(int j=trainSize; j<numInputs; j++){
            if(data[targetBaseIndex+j] == results[j])
                correctTest++;
            errorTest += pow(abs(data[targetBaseIndex+j] - results[j]),2);
        }
        fitness[i] = errorTrain;
        accuracy[i] = correctTest / (double)(numInputs - trainSize);
    }
    delete [] results;
}


void Manager::runCPUGeneration(int* data){
    // runCPUThread(data, results, 0, popSize);

    const int nThreads = 64;

    thread** t = new thread*[nThreads];

    int start = 0;
    for(int i=0; i<nThreads-1; i++){
        int end = (popSize/nThreads) * i;
        t[i] = new thread(&Manager::runCPUThread, this, data, start, end);
        start = end;
    }
    t[nThreads-1] = new thread(&Manager::runCPUThread, this, data, start, popSize);

    for(int i=0; i<nThreads; i++){
        t[i]->join();
        delete t[i];
    }
    delete [] t;
}


void Manager::runCPU(int numGen){
    int* data = (int*)dataLoader.getGPUData();

    for(int i=0; i<numGen; i++){
        runCPUGeneration(data);
        printGenerationStats(i);
        vector<int> doomedPool = population.tournamentSelection(fitness);
        population.applyGenOps(doomedPool);
    }
}

void Manager::runGPU(){
    cout << "Using GPU\n";

    // compShader.execShader(fitness, popSize, numInputs);

    for(int i=0; i<popSize; i++){
        cout << fitness[i] << " ";
    }
    cout << endl;
}

void Manager::validateGPU(){
    float* results = new float[popSize*numInputs];

    for(int i=0; i<popSize*numInputs; i++){
        results[i] = 0.1;
    }
    // cout << endl;

    compShader.execShader(results, popSize, numInputs);

    int differences = 0;
    for(int i=0; i<popSize*numInputs;i++){
        if(results[i] == 0.1)
            differences++;
    }
    cout << differences << " didn't execute\n";

    int* data = (int*)dataLoader.getGPUData();

    differences = 0;
    for(int t=0; t<popSize; t++){
        int treeSize = population.trees[t].size();
        Node* bottom = &population.trees[t][treeSize-1];

        for(int r=0; r<numInputs; r++){
            int cpuAns = execTree(bottom, data, r);
            if(cpuAns != (int)results[t*899 + r]){
                differences++;
                // cout << (int)results[t*899 + r] << " should be " << cpuAns << endl;
                // cout << "Mismatch with tree: ";
                // for(int i=population.startIndexes[t-1]+2; i < population.startIndexes[t]; i++){
                //     cout << population.trees[i] << " ";
                // }
                // cout << endl;
                // break;

            }
        }
    }
    cout << "Out of " << popSize * numInputs << " executions, " << differences << " mismatched\n";
    delete [] results;
}
