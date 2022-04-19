#pragma once

#include <string>
#include <fstream>

// In raw data 75 fields, if name is excluded. target value #58

// 8 float fields, 62 int fields (result included)
//      target value: 46 (I think)


class DataLoader{
private:
    static const int nRecords = 899;
    static const int nFields = 75;
    int nFieldsGPU = 0;

    int currRecords = 0;
    float* data = 0;

    int excludeFields[5] = {0, 1, 7, 44, 45};
    int floatFields[8] = {28, 29, 30, 39, 46, 48, 73, 74};

    // Combination of floats followed by ints, the GPU will know what to do
    int* gpuData = 0;

    void readRecord(std::ifstream &file, int i);

public:
    DataLoader(){
        data = new float[nFields*nRecords];

        nFieldsGPU = nFields;

        nFieldsGPU -= (sizeof(excludeFields)/sizeof(int));
    };

    ~DataLoader(){
        if(data)
            delete [] data;
        if(gpuData)
            delete [] gpuData;
    };

    void addFromFile(std::string filename, int numRecords);

    //populates gpuData
    // Disposes of useless fields and converts types where needed
    void makeGPUFormat();
    void* getGPUData(){return (void*)gpuData;};
    unsigned int sizeofGPUData(){return nFieldsGPU*nRecords*sizeof(int);};

    void dropRawData();

    void printStats();

};
