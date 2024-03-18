#include "src/reader.h"
#include <iostream>
using namespace std;

int main() {
    reader reader;
    reader.readAndParseNodes();
    reader.readAndParseEdges();
    auto graph = reader.getGraph();
    size_t count = 0;

    reader.maxFlowSingleCity("C_6");



}
