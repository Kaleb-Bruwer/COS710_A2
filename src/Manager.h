#pragma once

#include <vector>

#include "DataLoader.h"
#include "Logger.h"

#include <Population/Population.h>
#include <Parameters.h>

class Manager{
private:
    Logger logger;
    std::vector<ReportLine> stats;

    DataLoader dataLoader;
    Population population;

    int popSize;
    int numInputs = 899;
    int trainSize = TRAIN_COUNT;

    float* fitness = 0;
    float* accuracy = 0;

    float* trainAcc = 0;
    bool* calced = 0; //indicates which trees still have to be calculated

    // Looks at fitness and accuracy
    void logGeneration(int genNum);

    void runCPUGeneration(int* data);
    void runCPUThread(int* data, int start, int end);

public:
    Manager(){};
    ~Manager();
    void initialize(int popSize, int maxDepth = 10);
    void runCPU(int numGen, int runNumber = 1);

    void printInfo();

    // Returns last stat by default
    ReportLine getStat(int gen = -1);

};
