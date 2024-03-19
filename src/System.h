//
// Created by Rafa on 05/03/2024.
//

#include "../data_structures/Graph.h"
#include "Reservoir.h"
#include "City.h"
#include "Station.h"
#include "Pipe.h"
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
    unordered_map<string,Pipe> codesToPipe;


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

    void balanceLoad();

    void averageFlowPipes();


    void removeReservoir(const string& r);

    void removeFlowAlongPath(Vertex<string> *s, Vertex<string> *t, double f);

    void removeFlowAlongPath(Vertex<string> *s, double f);

    void updatePath(const string &u, const string &r);

    void removePS(const string& ps);

    void removePipe(const string &pipe);

    void removePipe(Graph<string> g, const string &pa, const string &pb);

    void runEdmondsKarp(const string &city);

    void runEdmondsKarp();

    void fillPipeMap();
};


#endif //DA2324_PRJ1_G15_4_PARSING_H
