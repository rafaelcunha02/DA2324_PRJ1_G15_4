//
// Created by Rafa on 05/03/2024.
//

#include "../data_structures/Graph.h"
#include "Reservoir.h"
#include "City.h"
#include "Station.h"
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

    unordered_map<string,Reservoir> codeToReservoir;
    unordered_map<string,City> codeToCity;
    unordered_map<string,Station> codeToStation;

    void edmondsKarp(Graph<string>& g, const string& source, const string& target);

public:
    reader();
    void readAndParseNodes();
    void readAndParseEdges();

    const Graph<string>& getGraph() const{
       return graph;
    }

    void maxFlowSingleCity(const string &City);

    void maxFlowEachCity();

    void setGraph(const Graph<string>& g){
        graph = g;
    }
};


#endif //DA2324_PRJ1_G15_4_PARSING_H
