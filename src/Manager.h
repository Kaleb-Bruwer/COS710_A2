#pragma once

#include "DataLoader.h"
#include <Population/Population.h>
#include "CompShader.h"
#include "Logger.h"

#include <Parameters.h>

class Manager{
private:
    Logger logger;

    DataLoader dataLoader;
    Population population;
    CompShader compShader;

    int popSize;
    int numInputs = 899;
    int trainSize = TRAIN_COUNT;

    float* fitness = 0;
    float* accuracy = 0;

    float* trainAcc = 0;

    // Looks at fitness and accuracy
    void logGeneration(int genNum);

    void runCPUGeneration(int* data);
    void runCPUThread(int* data, int start, int end);

public:
    Manager(){};
    ~Manager();
    void initialize(int popSize, int maxDepth = 10);
    void runCPU(int numGen, int runNumber = 1);
    void runGPU();

    // Compares GPU output to cpu's
    void validateGPU();
    void printInfo();

};
