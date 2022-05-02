#pragma once

#include "DataLoader.h"
#include <Population/Population.h>
#include "CompShader.h"

class Manager{
private:
    DataLoader dataLoader;
    Population population;
    CompShader compShader;

    int popSize;
    int numInputs = 899;
    int trainSize = 700;

    float* fitness = 0;
    float* accuracy = 0;

    // Looks at fitness and accuracy
    void printGenerationStats(int genNum);

    void runCPUGeneration(int* data);
    void runCPUThread(int* data, int start, int end);

public:
    Manager(){};
    ~Manager();
    void initialize(int popSize, int maxDepth = 10);
    void runCPU(int numGen);
    void runGPU();

    // Compares GPU output to cpu's
    void validateGPU();
    void printInfo();

};
