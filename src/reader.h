//
// Created by Rafa on 05/03/2024.
//

#include "../data_structures/Graph.h"
#include "Node.h"
#include <sstream>
#include <fstream>

#ifndef DA2324_PRJ1_G15_4_PARSING_H
#define DA2324_PRJ1_G15_4_PARSING_H


class reader {
private:
    Graph<Node> graph;

public:
    reader();
    void readAndParseNodes();

};


#endif //DA2324_PRJ1_G15_4_PARSING_H
