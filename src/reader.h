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
#include <unordered_map>

using namespace std;

#ifndef DA2324_PRJ1_G15_4_PARSING_H
#define DA2324_PRJ1_G15_4_PARSING_H


class reader {
private:
    Graph<string> graph;

    unordered_map<string,WaterReservoir> codeToReservoir;
    unordered_map<string,City> codeToCity;
    unordered_map<string,PumpingStation> codeToStation;

public:
    reader();
    void readAndParseNodes();
    void readAndParseEdges();

    const Graph<string>& getGraph() const{
       return graph;
    }

};


#endif //DA2324_PRJ1_G15_4_PARSING_H
