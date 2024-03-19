#include "src/System.h"
#include <iostream>
using namespace std;

int main() {
    System system;
    system.readAndParseNodes();
    system.readAndParseEdges();
    auto graph = system.getGraph();

    system.maxFlowEachCity();

    cout << endl << endl;


    system.fillPipeMap();

    system.removePS("PS_6");
    system.removePS("PS_6");
    system.removePS("PS_6");
    system.removePS("PS_6");
    system.removePS("PS_6");








    cout << endl;

    for (auto edge : graph.findVertex("C_1")->getIncoming()){
        cout << edge->getOrig()->getInfo() << endl;
    }






}


