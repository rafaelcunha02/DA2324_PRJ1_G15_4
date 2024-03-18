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


class System {
private:
    Graph<string> graph;

    unordered_map<string,Reservoir> codeToReservoir;
    unordered_map<string,City> codeToCity;
    unordered_map<string,Station> codeToStation;

    void edmondsKarp(Graph<string>& g, const string& source, const string& target);

public:
    System();
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

    void testAndVisit(queue<Vertex<string> *> &q, Edge<string> *e, Vertex<string> *w, double residual);

    bool findAugmentingPath(Graph<string> *g, Vertex<string> *s, Vertex<string> *t);

    double findMinResidualAlongPath(Vertex<string> *s, Vertex<string> *t);

    void augmentFlowAlongPath(Vertex<string> *s, Vertex<string> *t, double f);

    void enoughWater();
};


#endif //DA2324_PRJ1_G15_4_PARSING_H
