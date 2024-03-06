#include "src/reader.h"
#include <iostream>
using namespace std;

int main() {
    reader reader;
    reader.readAndParseNodes();
    reader.readAndParseEdges();
    auto graph = reader.getGraph();
    size_t count = 0;
    for (auto v : graph.getVertexSet()){
        count += v->getAdj().size();
    }

    cout << "number of edges: " << count;
}
