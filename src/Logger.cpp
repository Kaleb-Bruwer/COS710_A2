#include "Logger.h"

#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

void Logger::openFile(string filename){
    file.open(filename);
}


void Logger::writeHeader(string firstField){
    writeHeader(file, firstField);
}

void Logger::writeHeader(ostream &out, string firstField){
    out << fixed << left;
    out << setw(8) << firstField;
    out << setw(24) << "TRAINING ACC";
    out << setw(24) << "TESTING ACC";
    out << setw(36) << "FITNESS";
    out << setw(8) << "NODES";
    out << endl;

    out << setw(8) << " ";

    int w[] = {8,8,12};
    for(int s : w){
        out << setw(s) << "avg";
        out << setw(s) << "best";
        out << setw(s) << "dev";
    }
    out << endl;
}

void writeTuple(std::ostream& out, const std::tuple<float, float, float> t, int space){
    out << fixed << left;
    out.precision(2);
    out << setw(space) << get<0>(t);
    out << setw(space) << get<1>(t);
    out << setw(space) << get<2>(t);
}


ostream& operator<<(ostream& out, const ReportLine &l){
    out << fixed << left;
    out << setw(8) << l.generation;

    writeTuple(out, l.trainAcc);
    writeTuple(out, l.testAcc);
    writeTuple(out, l.fitness, 12);

    // out << get<0>(l.trainAcc) << "\t" << get<1>(l.trainAcc) << "\t" << get<2>(l.trainAcc) << "\t";
    // out << get<0>(l.testAcc) << "\t" << get<1>(l.testAcc) << "\t" << get<2>(l.testAcc) << "\t";
    // out << get<0>(l.fitness) << "\t" << get<1>(l.fitness) << "\t" << get<2>(l.fitness) << "\t";
    out << setw(8) << l.numNodes;
    out << endl;
    return out;
}


void Logger::writeLine(ReportLine l){
    file << l;
}

void Logger::closeFile(){
    file.close();
}
