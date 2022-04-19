#include <iostream>

#include "CompShader.h"
#include "DataLoader.h"
#include "Population.h"


/*
REMOVED 21 corrupt records from cleveland.data
*/

using namespace std;

int main(){
    // CompShader compShader;
    // compShader.loadData();
    // compShader.execShader();

    Population pop;
    pop.rampedFull(10000, 20);

    cout << "Generated population of 10000\n";
	return 0;
}
