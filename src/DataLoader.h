#pragma once

#include <string>
#include <fstream>

// 75 fields, if name is excluded

class DataLoader{
private:
    const int nRecords = 899;
    const int nFields = 75;

    int currRecords = 0;
    double* data;

    void readRecord(std::ifstream &file, int i);


public:
    DataLoader(){
        data = new double[nFields*nRecords];
    };

    void addFromFile(std::string filename, int numRecords);
    void printStats();

};
