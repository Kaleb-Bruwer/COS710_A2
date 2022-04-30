#include <iostream>

#include <chrono>
#include <random>

#include "Manager.h"
#include "cpuExec.h"

/*
REMOVED 21 corrupt records from cleveland.data

TODO:
* Multithread-compatible random     // default_random_engine engine(23456787);
* selection
* restructure trees on cpu side (separate vectors to speed up genetic operators)
* mutation, crossover
*/

using namespace std;

int main(){

    Manager manager;
    manager.initialize(10000, 8);

    manager.printInfo();
    auto start = chrono::high_resolution_clock::now();
    manager.runCPU(100);
    // manager.validateGPU();

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> diff = end - start;
    double ms = diff.count() * 1000;
    cout << "Time elapsed: " << ms << "ms\n";


	return 0;
}
