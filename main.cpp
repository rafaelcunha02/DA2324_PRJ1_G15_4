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
    system.removePipe(graph,"PS_44","PS_45");
    system.removePipe(graph,"PS_44","PS_45");
    system.removePipe(graph,"PS_44","PS_45");
    system.removePipe(graph,"PS_44","PS_45");
    system.removePipe(graph,"PS_44","PS_45");



}


