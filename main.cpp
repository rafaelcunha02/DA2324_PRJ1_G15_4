#include "src/reader.h"
#include <iostream>
using namespace std;

int main() {
    reader reader;
    reader.readAndParseNodes();
    auto graph = reader.getGraph();
    cout << graph.getNumVertex();
}
