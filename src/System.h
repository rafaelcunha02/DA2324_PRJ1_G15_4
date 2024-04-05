#ifndef DA2324_PRJ1_G15_4_PARSING_H
#define DA2324_PRJ1_G15_4_PARSING_H

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

/**
 * @brief Main class for the whole system.
 * @details Stores the information needed for the main features of the project to work.
 */
class System {

private:
    /*! @brief The graph that stores information of all the reservoirs, stations and cities (vertexes) as well as all the pipes (edges) that connect them.
     * @note The string stored on the vertexes info is the reservoir's, the station's or the city's code.
     */
    Graph<string> graph;

    /// @brief Unordered map that links a reservoir's code with a Reservoir object with the same code
    unordered_map<string,Reservoir> codeToReservoir;

    /// @brief Unordered map that links a city's code with a City object with the same code
    unordered_map<string,City> codeToCity;

    /// @brief Unordered map that links a station's code with a Station object with the same code
    unordered_map<string,Station> codeToStation;

    /// @brief Unordered map that links a string of the source's code concatenated with the target's code with a Pipe object with the same target and source
    unordered_map<string,Pipe> codesToPipe;

public:
    /*!
     * @brief Constructor of the System class.
     * @details Initializes the  member values with default values.
     * @note Time complexity: O(1).
     */
    System();


    /*!
     * @brief Getter for the system's graph.
     * @returns Graph<string> with the system's graph.
     */
    const Graph<string>& getGraph() const { return graph; }


    /*!
     * @brief Function to initialize the system.
     * @details Function that creates a super source and a super target and uses the Edmonds Karp algorithm to initialize the system.
     * @note Time complexity: O(V * E^2) where V equals the number of vertexes and E the number of edges in the graph.
     */
    void initialize();


    /*!
     * @brief Function that reads and parses all the nodes of the graph.
     * @details Reads a CSV file and parses the nodes of the graph while filling the unordered maps of the cities, the stations and the reservoirs.
     * @note Time complexity: O(n) where n equals the number of lines int the CSV file to be read.
     */
    void readAndParseNodes();

    /*!
     * @brief Function that reads and parses all the edges of the graph.
     * @details Reads a CSV file and parses the nodes of the graph while filling the unordered map of the pipes.
     * @note Time complexity: O(n) where n equals the number of lines int the CSV file to be read.
     */
    void readAndParseEdges();


    /*!
     * @brief Function to find and print the max-flow of a single city with the same code as the string City.
     * @param City String with a city's code.
     * @note Time complexity: O(1).
     */
    void maxFlowSingleCity(const string &City);

    /*!
     * @brief Function to print the max-flow of each city.
     * @note Time complexity: O(n) where n equals the number of cities.
     */
    void maxFlowEachCity();

    /*!
     * @brief Function to print the max-flow of the system.
     * @note Time complexity: O(n) where n equals the number of cities.
     */
    void maxFlowSystem();

    /*!
     * @brief Function to print the water deficit of each city in the system.
     * @note Time complexity: O(n) where n equals the number of cities.
     */
    void enoughWater();


    /*!
     * @brief Function that tests if a vertex is valid and if it is, visits it.
     * @param q queue<Vertex<string> *> with the queue of vertexes to be visited.
     * @param e Edge<string> with the edge to visit w.
     * @param w Vertex<string> with the vertex to be visited.
     * @param residual Double with the residual of the edge.
     * @note Time complexity: O(1).
     */
    void testAndVisit(queue<Vertex<string> *> &q, Edge<string> *e, Vertex<string> *w, double residual);

    /*!
     * @brief Function that finds the augmenting path between two vertexes.
     * @param g Graph<string> with the graph to be used.
     * @param s Vertex<string> with the source vertex.
     * @param t Vertex<string> with the target vertex.
     * @returns True if an augmenting path is found, false otherwise.
     * @note Time complexity: O(V + E) where V equals the number of vertexes and E the number of edges in the graph.
     */
    bool findAugmentingPath(Graph<string> &g, Vertex<string> *s, Vertex<string> *t);

    /*!
     * @brief Function that finds the minimum residual along the path.
     * @param s Vertex<string> with the source vertex.
     * @param t Vertex<string> with the target vertex.
     * @returns Double with the minimum residual along the path.
     * @note Time complexity: O(V) where V equals the number of vertexes in the graph.
     */
    double findMinResidualAlongPath(Vertex<string> *s, Vertex<string> *t);

    /*!
     * @brief Function that augments the flow along the path.
     * @param s Vertex<string> with the source vertex.
     * @param t Vertex<string> with the target vertex.
     * @param f Double with the flow to be augmented.
     * @note Time complexity: O(V) where V equals the number of vertexes in the graph.
     */
    void augmentFlowAlongPath(Vertex<string> *s, Vertex<string> *t, double f);

    /*!
     * @brief Function that finds the max-flow of the graph.
     * @param g Graph<string> with the graph to be used.
     * @param s Vertex<string> with the source vertex.
     * @param t Vertex<string> with the target vertex.
     * @returns Double with the max-flow of the graph.
     * @note Time complexity: O(V * E^2) where V equals the number of vertexes and E the number of edges in the graph.
     */
    void edmondsKarp(Graph<string> &g, const string &source, const string &target);


    /*!
     * @brief Function that removes a reservoir, simulates the impact of its removal and resets the graph to its original state.
     * @param r string with the reservoir's code.
     * @note Time complexity: O(V * E^2) where V equals the number of vertexes and E the number of edges in the graph.
     */
    void removeReservoir(const string& r);

    /*!
     * @brief Function that removes a reservoir permanently.
     * @param r string with the reservoir's code.
     * @note Time complexity: O(V * E^2) where V equals the number of vertexes and E the number of edges in the graph.
     */
    void permaremoveReservoir(const string& r);

    /*!
     * @brief Function that removes a vector of reservoirs.
     * @param vetor vector<string> with the reservoirs' codes.
     * @note Time complexity: O(V * E^2) where V equals the number of vertexes and E the number of edges in the graph.
     */
    void removeReservoirVector(const vector<string>& vetor);


    /*!
     * @brief Function that removes a station, simulates the impact of its removal and resets the graph to its original state.
     * @param ps string with the station's code.
     * @note Time complexity: O(V * E^2) where V equals the number of vertexes and E the number of edges in the graph.
     */
    void removePS(const string& ps);

    /*!
     * @brief Function that removes a station permanently.
     * @param ps string with the station's code.
     * @note Time complexity: O(V * E^2) where V equals the number of vertexes and E the number of edges in the graph.
     */
    void permaremovePS(const string &ps);

    /*!
     * @brief Function that removes a vector of stations.
     * @param vetor vector<string> with the stations' codes.
     * @note Time complexity: O(V * E^2) where V equals the number of vertexes and E the number of edges in the graph.
     */
    void removePSVector(const vector<string> &vetor);


    /*!
     * @brief Function that removes a pipe, simulates the impact of its removal and resets the graph to its original state.
     * @param pa string with the pipe's source's code.
     * @param pb string with the pipe's target's code.
     * @note Time complexity: O(V * E^2) where V equals the number of vertexes and E the number of edges in the graph.
     */
    void removePipe(const string &pa, const string &pb);

    /*!
     * @brief Function that removes a pipe permanently.
     * @param pa string with the pipe's source's code.
     * @param pb string with the pipe's target's code.
     * @note Time complexity: O(V * E^2) where V equals the number of vertexes and E the number of edges in the graph.
     */
    void permaremovePipe(const string &pa, const string &pb);

    /*!
     * @brief Function that removes a vector of pipes.
     * @param vetor vector<string> with the pipes' source's and target's codes concatenated.
     * @note Time complexity: O(V * E^2) where V equals the number of vertexes and E the number of edges in the graph.
     */
    void removePipeVector(const vector<string> &vetor);
};

#endif //DA2324_PRJ1_G15_4_PARSING_H
