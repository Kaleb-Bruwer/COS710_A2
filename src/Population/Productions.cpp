#include "Productions.h"

using namespace std;

ProductionTable* ProductionTable::getInstance(){
    if(!instance){
        instance = new ProductionTable();
    }
    return instance;
}

ProductionTable::ProductionTable(){
    table[RET_INT] = vector<vector<GenerateNode>>{
        vector<GenerateNode>{GenerateNode(VAR_INT)},
        vector<GenerateNode>{GenerateNode(RET_INT), GenerateNode(Node{0,1}), GenerateNode(RET_INT)}, //+
        vector<GenerateNode>{GenerateNode(RET_INT), GenerateNode(Node{0,2}), GenerateNode(RET_INT)}, //-
        vector<GenerateNode>{GenerateNode(RET_INT), GenerateNode(Node{0,3}), GenerateNode(RET_INT)}, //*
        vector<GenerateNode>{GenerateNode(RET_INT), GenerateNode(Node{0,4}), GenerateNode(RET_INT)}, //div
        vector<GenerateNode>{GenerateNode(RET_INT), GenerateNode(Node{0,5}), GenerateNode(RET_INT)}, //mod
        vector<GenerateNode>{GenerateNode(Node{0,6}), GenerateNode(RET_FLOAT)}, //round
        vector<GenerateNode>{GenerateNode(IF_INT)}
    };

    table[RET_FLOAT] = vector<vector<GenerateNode>>{
        vector<GenerateNode>{GenerateNode(VAR_FLOAT)},
        vector<GenerateNode>{GenerateNode(RET_FLOAT), GenerateNode(Node{0,33}), GenerateNode(RET_FLOAT)}, //+
        vector<GenerateNode>{GenerateNode(RET_FLOAT), GenerateNode(Node{0,34}), GenerateNode(RET_FLOAT)}, //-
        vector<GenerateNode>{GenerateNode(RET_FLOAT), GenerateNode(Node{0,35}), GenerateNode(RET_FLOAT)}, //*
        vector<GenerateNode>{GenerateNode(RET_FLOAT), GenerateNode(Node{0,36}), GenerateNode(RET_FLOAT)}, //div
        vector<GenerateNode>{GenerateNode(Node{0,37}), GenerateNode(RET_INT)}, //round
        vector<GenerateNode>{GenerateNode(IF_FLOAT)}
    };

    table[IF_INT] = vector<vector<GenerateNode>>{
        vector<GenerateNode>{GenerateNode(RET_BOOL), GenerateNode(RET_INT), GenerateNode(RET_INT)}
    };

    table[IF_FLOAT] = vector<vector<GenerateNode>>{
        vector<GenerateNode>{GenerateNode(RET_BOOL), GenerateNode(RET_FLOAT), GenerateNode(RET_FLOAT)}
    };

    table[VAR_INT] = vector<vector<GenerateNode>>{
        vector<GenerateNode>{GenerateNode(Node{1,0})},
        vector<GenerateNode>{GenerateNode(Node{2,0})}
    };

    table[VAR_FLOAT] = vector<vector<GenerateNode>>{
        vector<GenerateNode>{GenerateNode(Node{3,0})}
    };

    // TODO: add RET_BOOL once boolean operators are there

    table[RET_BOOL] = vector<vector<GenerateNode>>{
        vector<GenerateNode>{GenerateNode(RET_INT), GenerateNode(Node{0,17}), GenerateNode(RET_INT)}, //int == int
        vector<GenerateNode>{GenerateNode(RET_FLOAT), GenerateNode(Node{0,18}), GenerateNode(RET_FLOAT)}//float == float
    };
}

vector<GenerateNode> ProductionTable::getProduction(enum Production p, unsigned char codon){
    int numOptions = table[p].size();
    int choose = codon % numOptions;
    return table[p][choose];
}
