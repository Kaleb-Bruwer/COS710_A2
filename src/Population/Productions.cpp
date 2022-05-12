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
        vector<GenerateNode>{GenerateNode(Node{0,1}), GenerateNode(RET_INT), GenerateNode(RET_INT)}, //+
        vector<GenerateNode>{GenerateNode(Node{0,2}), GenerateNode(RET_INT), GenerateNode(RET_INT)}, //-
        vector<GenerateNode>{GenerateNode(Node{0,3}), GenerateNode(RET_INT), GenerateNode(RET_INT)}, //*
        vector<GenerateNode>{GenerateNode(Node{0,4}), GenerateNode(RET_INT), GenerateNode(RET_INT)}, //div
        vector<GenerateNode>{GenerateNode(Node{0,5}), GenerateNode(RET_INT), GenerateNode(RET_INT)}, //mod
        vector<GenerateNode>{GenerateNode(Node{0,6}), GenerateNode(RET_FLOAT)}, //round
        vector<GenerateNode>{GenerateNode(IF_INT)}
    };

    table[RET_FLOAT] = vector<vector<GenerateNode>>{
        vector<GenerateNode>{GenerateNode(VAR_FLOAT)},
        vector<GenerateNode>{GenerateNode(Node{0,33}), GenerateNode(RET_FLOAT), GenerateNode(RET_FLOAT)}, //+
        vector<GenerateNode>{GenerateNode(Node{0,34}), GenerateNode(RET_FLOAT), GenerateNode(RET_FLOAT)}, //-
        vector<GenerateNode>{GenerateNode(Node{0,35}), GenerateNode(RET_FLOAT), GenerateNode(RET_FLOAT)}, //*
        vector<GenerateNode>{GenerateNode(Node{0,36}), GenerateNode(RET_FLOAT), GenerateNode(RET_FLOAT)}, //div
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
        vector<GenerateNode>{GenerateNode(Node{0,17}), GenerateNode(RET_INT), GenerateNode(RET_INT)}, //int == int
        vector<GenerateNode>{GenerateNode(Node{0,18}), GenerateNode(RET_FLOAT), GenerateNode(RET_FLOAT)}//float == float
    };
}

Node ProductionTable::makeIntInput(unsigned char codon){
    int val = (codon >> 1) % 61;
    if(val >= 46)
        val++;
    return Node{2, val};
}


vector<GenerateNode> ProductionTable::getProduction(enum Production p,
        unsigned char codon, bool forceTerminate){
    if(forceTerminate && (p == RET_INT || p == RET_FLOAT || p == RET_BOOL))
        return table[p][0];


    int numOptions = table[p].size();
    int choose = codon % numOptions;

    auto result = table[p][choose];
    if(p == VAR_INT){
        // codon is bit-shifted since first bit determined int or input
        if(result[0].node.type == 1){//const
            choose = (codon >> 1) % 64;
            result[0].node.setConst(choose - 32);
        }
        else{//input
            result[0].node = makeIntInput(codon);
        }
    }
    else if(p == VAR_FLOAT){
        result[0].node.val = codon % 8;
    }

    return result;
}
