#include "Manager.h"

void Manager::initialize(){
    // Load data
    dataLoader.addFromFile("../data/cleveland.data", 282);
    dataLoader.addFromFile("../data/hungarian.data", 294);
    dataLoader.addFromFile("../data/long-beach-va.data", 200);
    dataLoader.addFromFile("../data/switzerland.data", 123);
    dataLoader.makeGPUFormat();
    dataLoader.dropRawData();

    // Generate initial population
    population.rampedFull(1000, 15);

    // Initialise shader
    compShader.initialize();
    compShader.loadData(dataLoader.getGPUData(), dataLoader.sizeofGPUData());

}
