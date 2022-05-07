#pragma once

#include <fstream>
#include <string>
#include <tuple>

struct ReportLine{
    int generation;

    // avg, best, std dev
    std::tuple<float, float, float> trainAcc;
    std::tuple<float, float, float> testAcc;
    std::tuple<float, float, float> fitness;
    int numNodes;

    friend std::ostream& operator<<(std::ostream& out, const ReportLine &r);
};

void writeTuple(std::ostream& out, const std::tuple<float, float, float> t, int space = 8);

class Logger{
private:
    std::ofstream file;
public:
    void openFile(std::string filename = "results.txt");
    void writeHeader(std::string firstField = "GEN");
    void writeHeader(std::ostream& out, std::string firstField = "GEN");
    void writeLine(ReportLine line);
    void closeFile();
};
