#include "src/System.h"
#include <iostream>
using namespace std;

int main() {
    System system;
    system.readAndParseNodes();
    system.readAndParseEdges();
    auto graph = system.getGraph();


    cout << endl << endl;

    system.fillPipeMap();
    system.removePS("PS_35");
}


