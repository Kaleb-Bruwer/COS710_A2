#include <iostream>

#include <chrono>

#include "Manager.h"
#include "cpuExec.h"

/*
REMOVED 21 corrupt records from cleveland.data
*/

using namespace std;

int main(){
    Manager manager;
    manager.initialize(1000);



    manager.printInfo();
    auto start = chrono::high_resolution_clock::now();
    manager.runGPU();

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> diff = end - start;
    double ms = diff.count() * 1000;
    cout << "Time elapsed: " << ms << "ms\n";


	return 0;
}
