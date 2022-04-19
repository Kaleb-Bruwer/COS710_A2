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
    pop.rampedFull(100, 10);

    // for(int i=0; i<100; i++){
    //     cout << pop.startIndexes[i] << " ";
    // }
    // cout << endl;

    // for(int i=0; i<16; i++){
    //     cout << pop.trees[i] << " ";
    // }
    // cout << endl;


    for(int i=pop.startIndexes[30]; i<pop.startIndexes[31]; i++){
        cout << pop.trees[i] << " ";
    }
    cout << endl;


    // cout << "Generated population of 10000\n";
	return 0;
}
