#pragma once

#include <string>
#include <fstream>

// 75 fields, if name is excluded

class DataLoader{
private:
    static const int nRecords = 899;
    static const int nFields = 75;
    int nFieldsGPU = 0;

    int currRecords = 0;
    float* data = 0;

    int excludeFields[5] = {0, 1, 7, 44, 45};
    int floatFields[8] = {28, 29, 30, 39, 46, 48, 73, 74};

    // 0: short, 1:float16, 2:exclude
    char formatLookup[nFields] = {0};

    // Combination of floats followed by ints, the GPU will know what to do
    int* gpuData = 0;

    void readRecord(std::ifstream &file, int i);

public:
    DataLoader(){
        data = new float[nFields*nRecords];

        nFieldsGPU = nFields;

        for(int e : excludeFields){
            formatLookup[e] = 2;
            nFieldsGPU--;
        }

        for(int f : floatFields){
            formatLookup[f] = 1;
        }
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
