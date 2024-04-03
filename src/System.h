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

/**
 * @class System
 * @brief Class that represents the system and all its components
 */

class System {
private:

    /**
     * @brief Graph that represents the system
     */
    Graph<string> graph;


    // Maps to store the objects created
    // Estes mapas na sua maioria vao ser necessarios para resetar informação do grafo

    /**
 * @brief Map that links the reservoirs to their codes
 */
    unordered_map<string,Reservoir> codeToReservoir;

    /**
     * @brief Map that links the cities to their codes
     */
    unordered_map<string,City> codeToCity;

    /**
     * @brief Map that links the stations to their codes
     */
    unordered_map<string,Station> codeToStation;

    /**
     * @brief Map that links the pipes to their codes
     */
    unordered_map<string,Pipe> codesToPipe;


public:

    System();


    /**
    * @brief Function to read and parse the nodes from the CSV files
    * @details This function reads the nodes from the CSV files and creates the objects
    * Time complexity: O(N), N = number of lines in all the parsed CSV files
    */
    void readAndParseNodes();


    /**
     * @brief Function to read and parse the edges from the CSV files
     * @details This function reads the edges from the CSV files and creates the objects
     * Time complexity: O(N), N = number of lines in the parsed CSV files
     */
    void readAndParseEdges();


    /**
     * @brief Getter to the graph
     * @return Graph that represents the system
     */
    const Graph<string>& getGraph() const{
       return graph;
    }

    /**
     * @brief Getter to the reservoirs
     * @details
     */
    void maxFlowSingleCity(const string &City);

    void maxFlowEachCity();

    void testAndVisit(queue<Vertex<string> *> &q, Edge<string> *e, Vertex<string> *w, double residual);

    bool findAugmentingPath(Graph<string> *g, Vertex<string> *s, Vertex<string> *t);

    double findMinResidualAlongPath(Vertex<string> *s, Vertex<string> *t);

    void augmentFlowAlongPath(Vertex<string> *s, Vertex<string> *t, double f);

    void enoughWater();

    void balanceLoad();

    void averageFlowPipes();


    void removeReservoir(const string& r);

    void removePS(const string& ps);



    /**
 * @brief Initializes the system by creating Super source and Super target, running EdmondsKarp for the first time, and filling the Pipes map.
 *
 * This function is supposed to be called at the beginning. It creates the Super source and Super target, runs EdmondsKarp for the first time, and finally fills the Pipes map.
 * The Pipes map associates the source code concatenated with the target code to the Pipe information.
 * The readAndParseEdges function does not immediately add the Pipes to this map, because only after running EdmondsKarp will they have flow.
 * It is useful to have the flow stored for other functions, because not only will we have to reset the graph, but it will also facilitate the search for information.
 * Instead of searching in the graph, we will just pull from the map directly. Pulling from the map, if I'm not mistaken, has a time complexity of O(1).
 *
 * @note This function also fills a file as they instruct, with the initial max flow of each city, without taking into account removals, etc.
 */
    void initialize();

    void removePipe(const string &pa, const string &pb);

    void maxFlowSystem();

    void removeReservoirVector(const vector<string>& vector);

    void permaremoveReservoir(const string& r);

    void permaremovePS(const string &ps);

    void removePSVector(const vector<string> &vetor);

    void permaremovePipe(const string &pa, const string &pb);

    void removePipeVector(const vector<string> &vetor);

    void edmondsKarp(Graph<string> *g, const string &source, const string &target);

    void edmondsKarp(Graph<string> &g, const string &source, const string &target);

    bool findAugmentingPath(Graph<string> &g, Vertex<string> *s, Vertex<string> *t);
};


#endif //DA2324_PRJ1_G15_4_PARSING_H
