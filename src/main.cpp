#include <iostream>

#include <chrono>
#include <random>
#include <vector>

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

const int numRuns = 10;

int main(){
    auto startALL = chrono::high_resolution_clock::now();

    vector<ReportLine> results;

    for(int i=0; i<numRuns; i++){
        srand(i*123 + 111);
        auto start = chrono::high_resolution_clock::now();

        Manager manager;
        manager.initialize(POPULATION, INIT_DEPTH);

        manager.runCPU(GENERATIONS, i);

        ReportLine r = manager.getStat();
        r.generation = i;
        results.push_back(r);

        auto end = chrono::high_resolution_clock::now();

        chrono::duration<double> diff = end - start;
        double seconds = diff.count();
        cout.precision(2);
        cout << "Run " << i << " runtime: " << seconds << "s\n\n";
    }

    // Write combined results
    Logger logger;
    logger.openFile("../Results/Results_combined.txt");
    logger.writeHeader("RUN");
    logger.writeHeader(cout, "RUN");
    for(ReportLine& r : results){
        logger.writeLine(r);
        cout << r;
    }
    logger.closeFile();



    auto endALL = chrono::high_resolution_clock::now();

    chrono::duration<double> diff = endALL - startALL;
    int seconds = diff.count();
    cout << "Total program runtime: " << seconds << "s\n";


	return 0;
}
