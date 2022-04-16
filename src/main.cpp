#include <iostream>

#include "CompShader.h"
#include "DataLoader.h"

/*
REMOVED 21 corrupt records from cleveland.data
*/

using namespace std;

int main(){
    CompShader compShader;
    compShader.loadData();
    // compShader.execShader();

    // DataLoader loader;
    // loader.addFromFile("../data/cleveland.data", 282);
    // loader.addFromFile("../data/hungarian.data", 294);
    // loader.addFromFile("../data/long-beach-va.data", 200);
    // loader.addFromFile("../data/switzerland.data", 123);
    // loader.printStats();

	return 0;
}
