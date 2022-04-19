#pragma once

#include "DataLoader.h"
#include "Population.h"
#include "CompShader.h"

class Manager{
private:
    DataLoader dataLoader;
    Population population;
    CompShader compShader;

public:
    Manager(){};
    void initialize();

};
