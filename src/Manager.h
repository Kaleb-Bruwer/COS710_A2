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

public:
    Manager(){};
    void initialize(int popSize);
    void runCPU();
    void runGPU();

    // Compares GPU output to cpu's
    void validateGPU();
    void printInfo();

};
