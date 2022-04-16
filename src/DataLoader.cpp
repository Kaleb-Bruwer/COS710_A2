#include "DataLoader.h"

#include <sstream>
#include <iostream>
#include <stdio.h>

#include "Helpers.h"

using namespace std;

void DataLoader::addFromFile(std::string filename, int numRecords){
    ifstream file(filename);

    for(int i=0; i<numRecords; i++){
        readRecord(file, currRecords);
        currRecords++;
    }
}

void DataLoader::readRecord(ifstream &file, int r){
    stringstream recSS;

    // Every record is spread over 10 lines
    for(int i=0; i<10;i++){
        string line;
        getline(file,line);
        recSS << line << ' ';
    }

    for(int i=0; i<nFields; i++){
        string valStr;
        getline(recSS, valStr, ' ');

        // By-record ordering
        // data[r*nFields + i] = stof(valStr);

        // By-field ordering
        data[r + i*nRecords] = stod(valStr);

    }
}

void DataLoader::printStats(){
    for(int i=0; i<nFields; i++){
        double min = getMin(&data[i*nRecords], currRecords);
        double max = getMax(&data[i*nRecords], currRecords);
        double avg = getAvg(&data[i*nRecords], currRecords);

        int nonInt = countNonInts(&data[i*nRecords], currRecords);

        printf("val %d: (%.2f, %.2f, %.2f) \t\trange: %f non-int: %d\n", i+1, min, avg, max, max-min, nonInt);
    }
}
