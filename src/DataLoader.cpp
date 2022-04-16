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
    // data is ordered by field
    gpuData = new short[nFieldsGPU * nRecords];
    int gpuField = 0;

    for(int f = 0; f<nFields; f++){
        char format = formatLookup[f];

        switch(format){
            case 0:
                for(int i=0; i<nRecords; i++){
                    gpuData[gpuField*nRecords + i] = data[f*nRecords + i];
                }
                break;
            case 1:
                for(int i=0; i<nRecords; i++){
                    gpuData[gpuField*nRecords + i] = floatToFloat16(data[f*nRecords + i]);
                }
                break;
            case 2:
                continue;
        }
        gpuField++;
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
