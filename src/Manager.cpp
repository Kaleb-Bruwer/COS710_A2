#include "Manager.h"

#include <iostream>
#include <thread>
#include <string>

#include "cpuExec.h"
#include "Helpers.h"
#include <Fitness.h>

using namespace std;

#define castF(a) *(float*)(void*)(&a)

Manager::~Manager(){
    if(accuracy)
        delete [] accuracy;
    if(fitness)
        delete [] fitness;
    if(trainAcc)
        delete [] trainAcc;
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
    trainAcc = new float[p];

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

void Manager::logGeneration(int genNum){
    ReportLine record;
    record.generation = genNum;
    record.trainAcc = make_tuple(
        getAvg(trainAcc, popSize),
        getMax(trainAcc, popSize),
        getStdDev(trainAcc, popSize)
    );
    record.testAcc = make_tuple(
        getAvg(accuracy, popSize),
        getMax(accuracy, popSize),
        getStdDev(accuracy, popSize)
    );

    record.fitness = make_tuple(
        getAvg(fitness, popSize),
        getMin(fitness, popSize),
        getStdDev(fitness, popSize)
    );
    record.numNodes = population.numNodes;

    if(genNum % 10 == 0)
        cout << record;
    logger.writeLine(record);
    stats.push_back(record);
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
        accuracy[i] = hitrate(&data[targetBaseIndex+trainSize],
                &results[trainSize], numInputs - trainSize);

        trainAcc[i] = hitrate(&data[targetBaseIndex], results, trainSize);

        // fitness[i] = mean_squared_error(&data[targetBaseIndex], results, trainSize);
        fitness[i] = 1 - trainAcc[i];
        fitness[i] += REGULARIZATION_WEIGHT * treeSize;

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


void Manager::runCPU(int numGen, int runNumber){
    string filename = "../Results/Results_run" + to_string(runNumber);
    filename += ".txt";
    cout << "Full results will be stored in: " << filename << endl;
    cout << "Terminal ouput is condensed\n";
    logger.openFile(filename);
    logger.writeHeader();
    logger.writeHeader(cout);

    int* data = (int*)dataLoader.getGPUData();

    for(int i=0; i<=numGen; i++){
        runCPUGeneration(data);
        logGeneration(i);
        vector<int> doomedPool = population.tournamentSelection(fitness);
        population.applyGenOps(doomedPool);
        population.recalcNumNodes();
    }
    logger.closeFile();
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

ReportLine Manager::getStat(int gen){
    if(gen < 0)
        return stats[stats.size()-1];
    return stats[gen];
}
