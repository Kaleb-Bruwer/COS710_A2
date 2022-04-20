#pragma once

#include "DataLoader.h"
#include "Population.h"
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
    void printInfo();

};