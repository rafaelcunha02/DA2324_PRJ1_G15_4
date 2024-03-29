#include "src/App.h"
#include "src/System.h"
#include <iostream>
using namespace std;

int main() {

     vector<string> vetor = {};



    System system;
    system.readAndParseNodes();
    system.readAndParseEdges();
    system.initialize();

    /*for (auto v : system.getGraph().getVertexSet()){
        if (v->getInfo()[0] == 'P'){
            vetor.push_back(v->getInfo());
        }
    }

    system.removeReservoirVector(vetor);
    system.removeReservoir("R_1");
    system.maxFlowSystem();


    system.removePSVector(vetor);*/
    App::run();

}


