//
// Created by Rafa on 05/03/2024.
//

#include "../data_structures/Graph.h"
#include "WaterReservoir.h"
#include "City.h"
#include "PumpingStation.h"
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

#ifndef DA2324_PRJ1_G15_4_PARSING_H
#define DA2324_PRJ1_G15_4_PARSING_H


class reader {
private:
    Graph<string> graph;

public:
    reader();
    void readAndParseNodes();

    const Graph<string>& getGraph() const{
       return graph;
    }

};


#endif //DA2324_PRJ1_G15_4_PARSING_H
