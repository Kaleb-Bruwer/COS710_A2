#include "DataLoader.h"

#include <sstream>
#include <iostream>
#include <stdio.h>

#include "Helpers.h"
#include "Float16.h"

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
        data[r + i*nRecords] = stof(valStr);

    }
}

void DataLoader::makeGPUFormat(){
    // data is ordered by field, with floats before ints

    // Data cast as int to ease iteration;
    gpuData =  new int[nFieldsGPU * nRecords];
    int gpuField = 0;

    for(int f : floatFields){
        memcpy(&gpuData[gpuField*nRecords], &data[f*nRecords], currRecords * sizeof(int));
        gpuField++;
    }

    for(int i=0; i<nFields; i++){
        // First check that i should be copied now
        bool found = false;
        for(int j : floatFields){
            if(j == i){
                found = true;
                break;
            }
        }
        if(!found){
            for(int j : excludeFields){
                if(j == i){
                    found = true;
                    break;
                }
            }
        }

        if(!found){
            memcpy(&gpuData[gpuField*nRecords], &data[i*nRecords], currRecords * sizeof(int));
            gpuField++;
        }
    }
}

void DataLoader::dropRawData(){
    delete [] data;
    data = 0;
}

void DataLoader::printStats(){
    // for(int i=0; i<nFields; i++){
    //     double min = getMin(&data[i*nRecords], currRecords);
    //     double max = getMax(&data[i*nRecords], currRecords);
    //     double avg = getAvg(&data[i*nRecords], currRecords);
    //
    //     int nonInt = countNonInts(&data[i*nRecords], currRecords);
    //
    //     printf("val %d: (%.2f, %.2f, %.2f) \t\trange: %f non-int: %d\n", i+1, min, avg, max, max-min, nonInt);
    // }
}