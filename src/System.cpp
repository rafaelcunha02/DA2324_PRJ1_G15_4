#include "System.h"
#include <string>
#include <iomanip>


using namespace std;

System::System() = default;

void System::readAndParseNodes() {

    ifstream file("../data/Reservoir.csv");
    string line;

    getline(file, line);
    while (getline(file, line)) {
        stringstream ss(line);
        string name, municipality, ident, code, del;


        double delivery;
        int id;

        getline(ss, name, ',');
        getline(ss, municipality, ',');
        getline(ss, ident, ',');
        getline(ss, code, ',');
        getline(ss, del, ',');


        delivery = stod(del);
        id = stoi(ident);


        Reservoir WR(name, municipality, id, code, delivery);
        codeToReservoir.insert({code, WR});
        cout << WR.getCode() << " " << WR.getDelivery() << endl;

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

    for (auto v : graph.getVertexSet()){
        if (v->getInfo()[0] == 'C'){
            cout << codeToCity.at(v->getInfo()).getCode() << " " << codeToCity.at(v->getInfo()).getDemand() << endl;
        }
    }

}


void System::readAndParseEdges() {


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




// Function to test the given vertex 'w' and visit it if conditions are met
void System::testAndVisit(std::queue< Vertex<string>*> &q, Edge<string> *e, Vertex<string> *w, double residual) {
// Check if the vertex 'w' is not visited and there is residual capacity
    if (! w->isVisited() && residual > 0) {
// Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}
// Function to find an augmenting path using Breadth-First Search
bool System::findAugmentingPath(Graph<string>& g, Vertex<string> *s, Vertex<string> *t) {
// Mark all vertices as not visited
    for(auto v : g.getVertexSet()) {
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
// Process incoming edges
        for(auto e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
// Return true if a path to the target is found, false otherwise
    return t->isVisited();
}





double System::findMinResidualAlongPath(Vertex<string> *s, Vertex<string> *t) {
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
void System::augmentFlowAlongPath(Vertex<string> *s, Vertex<string> *t, double f) {
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


void System::edmondsKarp(Graph<string>& g, const string& source, const string& target) {
// Find source and target vertices in the graph
    Vertex<string>* s = g.findVertex(source);
    Vertex<string>* t = g.findVertex(target);
// Validate source and target vertices
    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");
// Initialize flow on all edges to 0
    for (auto v : g.getVertexSet()) {
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }
// While there is an augmenting path, augment the flow along the path
    while( findAugmentingPath(g, s, t) ) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }
}



void System::initialize() {

    Reservoir superSource("superS", "", 0, "r_Super", INF);
    City superTarget("superT",0,"c_Super",INF,INF);

    graph.addVertex(superSource.getCode());
    graph.addVertex(superTarget.getCode());

    for (auto& v : graph.getVertexSet()){
        if (v->getInfo()[0] == 'R'){
            graph.addEdge(superSource.getCode(), v->getInfo(), codeToReservoir.at(v->getInfo()).getDelivery());
        }
        else if (v->getInfo()[0] == 'C'){
            graph.addEdge(v->getInfo(),superTarget.getCode(), codeToCity.at(v->getInfo()).getDemand());
        }
    }

    edmondsKarp(graph,superSource.getCode(),superTarget.getCode());

    for (auto v : graph.getVertexSet()){
        if (v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
            for (auto e : v->getAdj()){
                pair<string,string> par;
                par.first = e->getOrig()->getInfo();
                par.second = e->getDest()->getInfo();
                Pipe edge (par.first,par.second,e->getWeight(),e->getFlow());
                string identifier = par.first + par.second;

                codesToPipe.insert({identifier,edge});
            }
        }
    }

    std::ofstream file("../data/max_flow_output.csv", std::ios::trunc);
    std::ofstream file2("../data/max_flow_output.csv", std::ios::app);
    file2 << "CityCode" << "," << "Max Flow" << std::endl;

    for (auto v : graph.getVertexSet()){
        if (v->getInfo()[0] == 'c'){
            for (auto e : v->getIncoming()){
                codeToCity.at(e->getOrig()->getInfo()).setMaxFlow(e->getFlow());
                file2 << e->getOrig()->getInfo() << "," << codeToCity.at(e->getOrig()->getInfo()).getMaxFlow() << std::endl;
            }
        }
    }

}

void System::maxFlowSingleCity(const string &city) {


    cout << "+--------------+--------------+" << endl;
    cout << "| City Code    | Max Flow     |" << endl;
    cout << "+--------------+--------------+" << endl;

    if (codeToCity.find(city) != codeToCity.end()){
        double maxflow = codeToCity.at(city).getMaxFlow();


        cout << "| " << setw(12) << std::left << codeToCity.at(city).getCode() << " | " << setw(12) << maxflow << " |" << endl;
        cout << "+--------------+--------------+" << endl;
    }
    else{
        cout << "Invalid choice, please try again with an existing city code.";
    }

}


void System::maxFlowEachCity(){
    cout << "+--------------+--------------+" << endl;
    cout << "| City Code    | Max Flow     |" << endl;
    cout << "+--------------+--------------+" << endl;


    for (const auto& par : codeToCity){
        auto city = par.first;
        double maxflow = codeToCity.at(city).getMaxFlow();


        cout << "| " << setw(12) << std::left << codeToCity.at(city).getCode() << " | " << setw(12) << maxflow << " |" << endl;
        cout << "+--------------+--------------+" << endl;
    }

}

void System::maxFlowSystem(){
    double tot = 0;
    for (const auto& par : codeToCity){
        tot += par.second.getMaxFlow();
    }

    cout << "The maximum flow for this network is " << tot << "m^3/sec";
}

void System::enoughWater() {
    cout << "+--------------+-----------------------+" << endl;
    cout << "| City Code    | Water Deficit         |" << endl;
    cout << "+--------------+-----------------------+" << endl;

    bool enough = true;
    for (const auto& par : codeToCity) {
        auto maxflow = par.second.getMaxFlow();
        if (maxflow < par.second.getDemand()) {
            enough = false;
            cout << "| " << setw(12) << left << par.second.getCode() << " | Deficit: " << setw(12) << left << (par.second.getDemand() - maxflow) << " |" << endl;
        }
    }

    cout << "+--------------+-----------------------+" << endl;

    if (enough) {
        cout << endl << "This network can successfully meet the water needs of every delivery site." << endl;
    } else {
        cout << endl << "This network cannot meet the water needs of every delivery site." << endl;
    }
}



void System::averageFlowPipes(){

    double total = 0;
    double totalmedias = 0;
    double count = 0;
    for (auto v : graph.getVertexSet()){
        for (auto e : v->getAdj()){

            auto source = e->getOrig()->getInfo();
            auto target = e->getDest()->getInfo();

            cout << "Pipe from " << source << " to " << target << ": "
            << e->getFlow() << "/" << e->getWeight()
            << " (" << (e->getFlow()/e->getWeight()) * 100 << "%)" << endl;
            count++;
            total += e->getFlow();
            totalmedias += (e->getFlow()/e->getWeight()) * 100;

        }
    }

    cout << "---------------------";
    cout << "Final stats--------------------" << endl;
    cout << "Average flow per pipe: " << total/count << endl;
    cout << "Average percentage of flow per pipe: " << totalmedias / count << endl;


    //variancia
    double n = 0;
    double sum = 0;
    double sum_sqr = 0;

    for (auto v: graph.getVertexSet()){
        for (auto e : v->getAdj()){

            n = n+1;
            sum = sum + e->getFlow();
            sum_sqr = sum_sqr + (e->getFlow() * e->getFlow());
        }
    }

    auto mean = sum/n;
    auto variance = (sum_sqr - sum*mean)/(n-1);

    cout << "Variance: " << variance;

}


void System::removeReservoir(const string& r){

    if (codeToReservoir.find(r) == codeToReservoir.end()){
        cout << "The chosen reservoir, '" << r << "'does not exist" << endl;
        cout << "Please try again with valid input." << endl;
        return;
    }




    double peso;
    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == r){
            for (auto edge : v->getIncoming()){
                peso = edge->getWeight();
                edge->setWeight(0);
            }
        }
    }

    edmondsKarp(graph,"r_Super","c_Super");

    cout << "On removal of the following Reservoir: " << r;
    cout << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;
    cout << "| City               | Before Removal | After Removal | Difference   |" << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

    for (auto v : graph.getVertexSet()) {
        if (v->getInfo()[0] == 'C') {
            double delafter = 0;
            for (auto edge : v->getIncoming()) {
                delafter += edge->getFlow();
            }
            double before = codeToCity.at(v->getInfo()).getMaxFlow();
            double difference = delafter - before;
            cout << "| " << left << setw(19) << v->getInfo() << "| " << setw(13) << before << "  | " << setw(13) << delafter << " | " << setw(13) << difference << "|" << endl;
        }
    }
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == r){
            for (auto edge : v->getIncoming()){
                edge->setWeight(peso);
            }
        }
    }
}


void System::permaremoveReservoir(const string& r){

    double peso;
    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == r){
            for (auto edge : v->getIncoming()){
                peso = edge->getWeight();
                edge->setWeight(0);
            }
        }
    }

    edmondsKarp(graph,"r_Super","c_Super");

    cout << "On removal of the following Reservoir: " << r;
    cout << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;
    cout << "| City               | Before Removal | After Removal | Difference   |" << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

    for (auto v : graph.getVertexSet()) {
        if (v->getInfo()[0] == 'C') {
            double delafter = 0;
            for (auto edge : v->getIncoming()) {
                delafter += edge->getFlow();
            }
            double before = codeToCity.at(v->getInfo()).getMaxFlow();
            codeToCity.at(v->getInfo()).setMaxFlow(delafter);
            double difference = delafter - before;
            cout << "| " << left << setw(19) << v->getInfo() << "| " << setw(13) << before << "  | " << setw(13) << delafter << " | " << setw(13) << difference << "|" << endl;
        }
    }
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

}

void System::removeReservoirVector(const vector<string>& vetor) {

    auto map2 = codeToReservoir;
    auto map3 = codeToCity;

    vector<pair<string,double>> removed;

    for (const auto& r : vetor){
        if (codeToReservoir.find(r) != codeToReservoir.end()){
            permaremoveReservoir(r);
            codeToReservoir = map2;
            removed.emplace_back(r, codeToReservoir.at(r).getDelivery());
        }
        else{
            cout << "The chosen reservoir, '" << r << "'does not exist" << endl;
            cout << "Skipping..." << endl << endl;
        }

    }

    codeToCity = map3;
    //reset the graph using the "removed" vector of pairs to reset the weights
    for (auto v : graph.getVertexSet()){
        if (v->getInfo()[0] == 'r'){
            for (auto e : v->getAdj()){
                for (auto& p : removed){
                    if (e->getDest()->getInfo() == p.first){
                        e->setWeight(p.second);
                        break;
                    }
                }
            }
            break;
        }
    }

}


void System::removePS(const string& ps){

    if (codeToStation.find(ps) == codeToStation.end()){
        cout << "The chosen Station, '" << ps << "'does not exist" << endl;
        cout << "Please try again with valid input." << endl;
        return;
    }

    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == ps){
            for (auto e : v->getIncoming()){
                e->setWeight(0);
            }
            break;
        }
    }

    edmondsKarp(graph,"r_Super","c_Super");


    cout << "On removal of the following pumping station: " << ps;
    cout << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;
    cout << "| City               | Before Removal | After Removal | Difference   |" << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

    for (auto v : graph.getVertexSet()) {
        if (v->getInfo()[0] == 'C') {
            double delafter = 0;
            for (auto edge : v->getIncoming()) {
                delafter += edge->getFlow();
            }
            double before = codeToCity.at(v->getInfo()).getMaxFlow();
            double difference = delafter - before;
            cout << "| " << left << setw(19) << v->getInfo() << "| " << setw(13) << before << "  | " << setw(13) << delafter << " | " << setw(13) << difference << "|" << endl;
        }
    }
    cout << "+--------------------+---------------+---------------+---------------+" << endl;


    for (auto v : graph.getVertexSet()){
        for (auto edge : v->getAdj()){
            if (v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
                string source = edge->getOrig()->getInfo();
                string target = edge->getDest()->getInfo();
                edge->setFlow(codesToPipe.at(source+target).getFlow());
                edge->setWeight(codesToPipe.at(source+target).getCapacity());
            }
        }
    }
}


void System::permaremovePS(const string& ps){

    //remover o peso da edges
    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == ps){
            for (auto e : v->getAdj()){
                e->setWeight(0);
            }
            for (auto e : v->getIncoming()){
                e->setWeight(0);
            }
            break;
        }
    }

    //Já podemos mandar o edmonds
    edmondsKarp(graph,"r_Super","c_Super");

    cout << "On removal of the following pumping station: " << ps;
    cout << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;
    cout << "| City               | Before Removal | After Removal | Difference   |" << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

    for (auto v : graph.getVertexSet()) {
        if (v->getInfo()[0] == 'C') {
            double delafter = 0;
            for (auto edge : v->getIncoming()) {
                delafter += edge->getFlow();
            }
            double before = codeToCity.at(v->getInfo()).getMaxFlow();
            codeToCity.at(v->getInfo()).setMaxFlow(delafter); //atualizar maxflow
            double difference = delafter - before;
            cout << "| " << left << setw(19) << v->getInfo() << "| " << setw(13) << before << "  | " << setw(13) << delafter << " | " << setw(13) << difference << "|" << endl;
        }
    }
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

}

void System::removePSVector(const vector<string>& vetor) {

    //guardar a informação
    auto map3 = codeToCity;

    vector<string> removed; //vetor para dar track ao que ja foi removido


    for (const auto& ps : vetor){
        if (codeToStation.find(ps) != codeToStation.end()){
            permaremovePS(ps); //remover
        }
        else{
            cout << "The chosen Pumping Station, '" << ps << "', does not exist" << endl;
            cout << "Skipping..." << endl << endl;
        }
    }

    //resetar a informação

    codeToCity = map3;

    for (auto v : graph.getVertexSet()){
        for (auto edge : v->getAdj()){
            if (v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
                string source = edge->getOrig()->getInfo();
                string target = edge->getDest()->getInfo();
                edge->setFlow(codesToPipe.at(source+target).getFlow());
                edge->setWeight(codesToPipe.at(source+target).getCapacity());
            }
        }
    }
}


void System::removePipe(const string& pa, const string& pb){

    if (codesToPipe.find(pa+pb) == codesToPipe.end()){
        cout << "The chosen Pipeline, '" << pa << " -> " << pb << "'does not exist" << endl;
        cout << "Please try again with valid input." << endl;
        return;
    }


    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == pa){
            for (auto edge : v->getAdj()){
                if (edge->getDest()->getInfo() == pb){
                    edge->setWeight(0);
                }
            }
            break;
        }
    }

    edmondsKarp(graph,"r_Super","c_Super");

    cout << "On removal of the following Pipeline: " << pa << " -> " << pb;


    cout << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;
    cout << "| City               | Before Removal | After Removal | Difference   |" << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

    for (auto v : graph.getVertexSet()) {
        if (v->getInfo()[0] == 'C') {
            double delafter = 0;
            for (auto edge : v->getIncoming()) {
                delafter += edge->getFlow();
            }
            double before = codeToCity.at(v->getInfo()).getMaxFlow();
            double difference = delafter - before;
            cout << "| " << left << setw(19) << v->getInfo() << "| " << setw(13) << before << "  | " << setw(13) << delafter << " | " << setw(13) << difference << "|" << endl;
        }
    }
    cout << "+--------------------+---------------+---------------+---------------+" << endl;


    for (auto v : graph.getVertexSet()){
        for (auto e : v->getAdj()){
            if (v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
                string source = e->getOrig()->getInfo();
                string target = e->getDest()->getInfo();
                e->setFlow(codesToPipe.at(source+target).getFlow());
                e->setWeight(codesToPipe.at(source+target).getCapacity());
            }
        }
    }
}


void System::permaremovePipe(const string& pa, const string& pb){

    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == pa){
            for (auto edge : v->getAdj()){
                if (edge->getDest()->getInfo() == pb){
                    edge->setWeight(0);
                }
            }
            break;
        }
    }

    edmondsKarp(graph,"r_Super","c_Super");

    cout << "On removal of the following Pipeline: " << pa << " -> " << pb;

    cout << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;
    cout << "| City               | Before Removal | After Removal | Difference   |" << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

    for (auto v : graph.getVertexSet()) {
        if (v->getInfo()[0] == 'C') {
            double delafter = 0;
            for (auto edge : v->getIncoming()) {
                delafter += edge->getFlow();
            }
            double before = codeToCity.at(v->getInfo()).getMaxFlow();
            codeToCity.at(v->getInfo()).setMaxFlow(delafter);
            double difference = delafter - before;
            cout << "| " << left << setw(19) << v->getInfo() << "| " << setw(13) << before << "  | " << setw(13) << delafter << " | " << setw(13) << difference << "|" << endl;
        }
    }
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

}


void System::removePipeVector(const vector<string>& vetor) {

    auto map2 = codeToReservoir; //guardar a informação
    auto map3 = codeToCity;


    for (const auto& pipe : vetor){
        if (codesToPipe.find(pipe) != codesToPipe.end()){
            permaremovePipe(codesToPipe.at(pipe).getSource(),codesToPipe.at(pipe).getTarget()); //remover
        }
        else{
            cout << "The chosen Pipeline, '" << pipe <<  "', does not exist" << endl;
            cout << "Skipping..." << endl << endl;
        }
    }


    //resetar a informação

    codeToCity = map3;

    for (auto v : graph.getVertexSet()){
        for (auto edge : v->getAdj()){
            if(v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
                string source = edge->getOrig()->getInfo();
                string target = edge->getDest()->getInfo();
                edge->setFlow(codesToPipe.at(source+target).getFlow());
                edge->setWeight(codesToPipe.at(source+target).getCapacity());
            }
        }
    }
}


void System::balanceLoad(){

}


