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

    system.enoughWater();

    system.averageFlowPipes();


}
