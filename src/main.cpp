#include <iostream>

#include <chrono>
#include <random>

#include <Parameters.h>
#include "Manager.h"
#include "cpuExec.h"
#include "Logger.h"

/*
REMOVED 21 corrupt records from cleveland.data

TODO:
    Do multiple runs
    Multi-run report
*/

using namespace std;

int main(){
    Manager manager;
    manager.initialize(POPULATION, INIT_DEPTH);

    manager.printInfo();
    auto start = chrono::high_resolution_clock::now();
    manager.runCPU(GENERATIONS);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> diff = end - start;
    double ms = diff.count() * 1000;
    cout << "Time elapsed: " << ms << "ms\n";


	return 0;
}
