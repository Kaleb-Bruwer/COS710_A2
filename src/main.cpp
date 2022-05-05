#include <iostream>

#include <chrono>
#include <random>

#include "Manager.h"
#include "cpuExec.h"

/*
REMOVED 21 corrupt records from cleveland.data

TODO:
    BOOL type + functions
    IF_EMPTY generation pattern (returns val if it isn't empty, else some subtree)
*/

using namespace std;

int main(){

    Manager manager;
    manager.initialize(5000, 5);

    manager.printInfo();
    auto start = chrono::high_resolution_clock::now();
    manager.runCPU(3000);
    // manager.validateGPU();

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> diff = end - start;
    double ms = diff.count() * 1000;
    cout << "Time elapsed: " << ms << "ms\n";


	return 0;
}
