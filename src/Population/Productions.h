#pragma once

#include "Node.h"

#include <vector>
#include <map>

enum Production{
    RET_INT,
    RET_FLOAT,
    IF_INT,
    IF_FLOAT,
    RET_BOOL,
    VAR_INT,
    VAR_FLOAT
};

struct GenerateNode{
    bool type; //0: Production, 1:Node
    enum Production production;
    Node node;

    GenerateNode(){};
    GenerateNode(enum Production p){
        type = false;
        production = p;
    };
    GenerateNode(Node n){
        type = true;
        node = n;
    };
};

class ProductionTable{
private:
    inline static ProductionTable* instance = 0;
    ProductionTable();

    // Every entry is a vector of vectors, with each sub-vector an individual production
    std::map<enum Production, std::vector<std::vector<GenerateNode>>> table;

    Node makeIntInput(unsigned char codon);

public:
    static ProductionTable* getInstance();

    std::vector<GenerateNode> getProduction(enum Production p, unsigned char codon, bool forceTerminate = false);
};
