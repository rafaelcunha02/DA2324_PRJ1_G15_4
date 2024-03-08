#include "reader.h"
#include <string>


using namespace std;

reader::reader() = default;

void reader::readAndParseNodes() {

    ifstream file("../data/Reservoir.csv");
    string line;

    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string name, municipality, ident, code, del;


        int delivery;
        int id;

        getline(ss, name, ',');
        getline(ss, municipality, ',');
        getline(ss, ident, ',');
        getline(ss, code, ',');
        getline(ss, del, ',');


        delivery = stoi(del);
        id = stoi(ident);


        Reservoir WR(name, municipality, id, code, delivery);
        codeToReservoir.insert({code, WR});

        cout << "Map size: " << codeToReservoir.size() << endl;
        graph.addVertex(WR.getCode());

    }

    //---------------------------------------//

    ifstream file2("../data/Cities.csv");
    string line2;

    getline(file2, line2);
    while (getline(file2, line2)) {
        stringstream ss(line2);
        string name, ident, Code, dem, pop;
        int id;
        int population;
        int demand;

        getline(ss, name, ',');
        getline(ss, ident, ',');
        getline(ss, Code, ',');
        getline(ss, dem, ',');
        getline(ss, pop, ',');

        demand = stoi(dem);
        id = stoi(ident);
        population = stoi(pop);
        City C(name, id, Code, demand, population);
        codeToCity.insert({Code, C});

        graph.addVertex(C.getCode());
    }

    //----------------------------------//

    ifstream file3("../data/Stations.csv");
    string line3;

    getline(file3, line3);
    while (getline(file3, line3)){
        stringstream ss(line3);
        string Ident, Code;
        int id;
        getline(ss, Ident, ',');
        getline(ss, Code, ',');

        id = stoi(Ident);


        Station PS(id, Code);
        codeToStation.insert({Code, PS});

        graph.addVertex(PS.getCode());

    }

}


void reader::readAndParseEdges() {

    ifstream file("../data/Pipes.csv");
    string line;

    getline(file, line);
    while (getline(file, line)){
        stringstream ss(line);
        string source, dest, capacidade, directed;
        double capacity;
        int isDirected;

        int id;
        getline(ss, source, ',');
        getline(ss, dest, ',');
        getline(ss, capacidade, ',');
        getline(ss, directed, ',');

        capacity = stod(capacidade);
        isDirected = stoi(directed);


        if(isDirected == 1){
            graph.addEdge(source, dest, capacity);
        }
        else{
            graph.addBidirectionalEdge(source,dest, capacity);
        }

    }

}




void testAndVisit(std::queue< Vertex<string>*> &q, Edge<string> *e, Vertex<string> *w, double residual) {
// Check if the vertex 'w' is not visited and there is residual capacity
    if (! w->isVisited() && residual > 0) {
// Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}


// Function to find an augmenting path using Breadth-First Search
bool findAugmentingPath(Graph<string> *g, Vertex<string> *s, Vertex<string> *t) {
// Mark all vertices as not visited
    for(auto v : g->getVertexSet()) {
        v->setVisited(false);
    }
// Mark the source vertex as visited and enqueue it
    s->setVisited(true);
    std::queue<Vertex<string> *> q;
    q.push(s);
// BFS to find an augmenting path
    while( ! q.empty() && ! t->isVisited()) {
        auto v = q.front();
        q.pop();
// Process outgoing edges
        for(auto e: v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }
    }
// Return true if a path to the target is found, false otherwise
    return t->isVisited();
}




double findMinResidualAlongPath(Vertex<string> *s, Vertex<string> *t) {
    double f = INF;
// Traverse the augmenting path to find the minimum residual capacity
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, e->getWeight() - e->getFlow());
            v = e->getOrig();
        }
        else {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }
// Return the minimum residual capacity
    return f;
}
// Function to augment flow along the augmenting path with the given flow value
void augmentFlowAlongPath(Vertex<string> *s, Vertex<string> *t, double f) {
// Traverse the augmenting path and update the flow values accordingly

    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        double flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + f);
            v = e->getOrig();
        }
        else {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}

void reader::edmondsKarp(Graph<std::string>& g, const std::string &source, const std::string &target) {
    // Find source and target vertices in the graph
    Vertex<string>* s = g.findVertex(source);
    Vertex<string>* t = g.findVertex(target);
// Validate source and target vertices
    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");

// While there is an augmenting path, augment the flow along the path
    while( findAugmentingPath( &g, s, t) ) {
        double f = findMinResidualAlongPath(s, t);
        auto delivery = codeToReservoir.at(source).getDelivery();
        if (f > delivery){
            f = delivery;
            delivery = 0;
        }
        else{
            delivery -= f;
        }

        codeToReservoir.at(source).setDelivery(delivery);
        augmentFlowAlongPath(s, t, f);
        if (delivery == 0){
            break;
        }
    }
}

void reader::maxFlowSingleCity(const string &city) {

    auto map2 = codeToReservoir;

    for (auto& v : graph.getVertexSet()){
        for (auto& edge : v->getAdj()){
            edge->setFlow(0);
        }
    }

    for (auto& v : graph.getVertexSet()){
        if (v->getInfo()[0] == 'R'){
            edmondsKarp(graph, v->getInfo(), city);
        }
    }

    double count = 0;
    for (auto& Edge : graph.findVertex(city)->getIncoming()){
        count += Edge->getFlow();

    }

    codeToReservoir = map2;
    cout << "Max flow for " << codeToCity.at(city).getName() << ": " << count << endl;
}


void reader::maxFlowEachCity(){

    for (auto v : graph.getVertexSet()){
        if (v->getInfo()[0] == 'C'){
            maxFlowSingleCity(v->getInfo());
        }
    }
}

