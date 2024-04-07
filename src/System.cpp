#include "System.h"
#include <string>
#include <iomanip>


using namespace std;


/**
 * @brief Default constructor
 */
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

        //cria Cidade, adiciona ao grafo de strings, associa a string à informação num mapa

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


        //cria Pumping Station, adiciona ao grafo de strings, associa a string à informação num mapa

        Station PS(id, Code);
        codeToStation.insert({Code, PS});

        graph.addVertex(PS.getCode());

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
            Pipe edge (source,dest, capacity, 0, isDirected);
            codesToPipe.insert({source+dest,edge});
        }
        else{
            graph.addBidirectionalEdge(source,dest, capacity);
            Pipe edge (source,dest, capacity, 0, isDirected);
            codesToPipe.insert({source+dest,edge});
            codesToPipe.insert({dest+source,edge});

        }
    }

}




void System::testAndVisit(std::queue< Vertex<string>*> &q, Edge<string> *e, Vertex<string> *w, double residual) {
    if (! w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

bool System::findAugmentingPath(Graph<string>& g, Vertex<string> *s, Vertex<string> *t) {
    for(auto v : g.getVertexSet()) {
        v->setVisited(false);
    }
    s->setVisited(true);
    std::queue<Vertex<string> *> q;
    q.push(s);

    while( ! q.empty() && ! t->isVisited()) {
        auto v = q.front();
        q.pop();

        for(auto e: v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }

        for(auto e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }

    return t->isVisited();
}


double System::findMinResidualAlongPath(Vertex<string> *s, Vertex<string> *t) {
    double f = INF;

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

    return f;
}

void System::augmentFlowAlongPath(Vertex<string> *s, Vertex<string> *t, double f) {

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

    Vertex<string>* s = g.findVertex(source);
    Vertex<string>* t = g.findVertex(target);

    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");

    for (auto v : g.getVertexSet()) {
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }

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

    //cria a supersource e a supertarget
    for (auto& v : graph.getVertexSet()){
        if (v->getInfo()[0] == 'R'){
            graph.addEdge(superSource.getCode(), v->getInfo(), codeToReservoir.at(v->getInfo()).getDelivery());
            Pipe edge (superSource.getCode(),v->getInfo(), codeToReservoir.at(v->getInfo()).getDelivery(), 0, 1);
            codesToPipe.insert({superSource.getCode()+v->getInfo(),edge});
        }
        else if (v->getInfo()[0] == 'C'){
            graph.addEdge(v->getInfo(),superTarget.getCode(), codeToCity.at(v->getInfo()).getDemand());
            Pipe edge (v->getInfo(),superTarget.getCode(), codeToCity.at(v->getInfo()).getDemand(), 0, 1);
            codesToPipe.insert({v->getInfo()+superTarget.getCode(),edge});
        }
    }

    edmondsKarp(graph,superSource.getCode(),superTarget.getCode());

    //esta parte preenche os flows do mapa das Pipes
    for (auto v : graph.getVertexSet()){
        if (v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
            for (auto e : v->getAdj()){
                pair<string,string> par;
                par.first = e->getOrig()->getInfo();
                par.second = e->getDest()->getInfo();
                string identifier = par.first + par.second; //concatenar os codigos dos vertices
                codesToPipe.at(identifier).setFlow(e->getFlow()); //associar o flow à informação
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


//funçao que encontra o maxflow para cada cidade

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

    vector<std::pair<std::string, City>> cityVector(codeToCity.begin(), codeToCity.end());

    sort(cityVector.begin(), cityVector.end(), [](const pair<string, City>& a, const pair<string, City>& b) {
        int cityNumA = stoi(a.first.substr(2));
        int cityNumB = stoi(b.first.substr(2));
        return cityNumA < cityNumB;
    });

    for (const auto& par : cityVector){
        auto city = par.first;
        double maxflow = par.second.getMaxFlow();

        cout << "| " << setw(12) << std::left << city << " | " << setw(12) << maxflow << " |" << endl;
        cout << "+--------------+--------------+" << endl;
    }
}


//printa o somatório de todos os max flows
void System::maxFlowSystem(){
    double tot = 0;
    for (const auto& par : codeToCity){
        tot += par.second.getMaxFlow();
    }

    cout << "The maximum flow for this network is " << tot << "m^3/sec";
}



void System::enoughWater() {
    cout << "+--------------+-----------------------+--------------+--------------+" << endl;
    cout << "| City Code    | Water Deficit         | Demand       | Flow         |" << endl;
    cout << "+--------------+-----------------------+--------------+--------------+" << endl;


    std::vector<std::pair<std::string, City>> cityVector(codeToCity.begin(), codeToCity.end());


    sort(cityVector.begin(), cityVector.end(), [](const pair<string, City>& a, const pair<string, City>& b) {
        int cityNumA = stoi(a.first.substr(2));
        int cityNumB = stoi(b.first.substr(2));
        return cityNumA < cityNumB;
    });

    bool enough = true;

    for (const auto& par : cityVector) {
        auto maxflow = par.second.getMaxFlow();
        auto demand = par.second.getDemand();
        if (maxflow < demand) {
            enough = false;
            cout << "| " << setw(12) << left << par.first << " | Deficit: " << setw(12) << left << (demand - maxflow) << " | " << setw(12) << left << demand << " | " << setw(12) << left << maxflow << " |" << endl;
        }
    }

    cout << "+--------------+-----------------------+---------------+-------------+" << endl;

    if (enough) {
        cout << endl << "This network can successfully meet the water needs of every delivery site." << endl;
    } else {
        cout << endl << "This network cannot meet the water needs of every delivery site." << endl;
    }
}



void System::removeReservoir(const string& r){


    //verificar se o reservatorio existe
    if (codeToReservoir.find(r) == codeToReservoir.end()){
        cout << "The chosen reservoir, '" << r << "'does not exist" << endl;
        cout << "Please try again with valid input." << endl;
        return;
    }

    //remover o peso da edge da SuperSource para esse reservatório
    //isto impede a passagem de flow para esse reservatório
    //bottleneck = 0 portanto
    double peso;
    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == r){
            for (auto edge : v->getIncoming()){
                peso = edge->getWeight(); //guardar o peso da edge que vai ser removida
                edge->setWeight(0); //só vai haver uma incoming edge para cada reservatorio (a da supersource)
                //isto faz com que a operação seja relativamente rápida
            }
        }
    }


    //correr o edmondsKarp para atualizar o flow
    edmondsKarp(graph,"r_Super","c_Super");


    //printar a diferença de flow antes e depois da remoção

    cout << "On removal of the following Reservoir: " << r;
    cout << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;
    cout << "| City               | Before Removal | After Removal | Difference   |" << endl;
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

    for (auto v : graph.getVertexSet()) {
        if (v->getInfo()[0] == 'C') {
            double delafter = 0;
            for (auto edge : v->getIncoming()) {
                delafter += edge->getFlow(); //contagem do flow atual através das incoming edges
            }
            double before = codeToCity.at(v->getInfo()).getMaxFlow(); //no initialize(), definimos o maxflow de cada cidade para o maxflow default do dataset
            double difference = delafter - before;
            cout << "| " << left << setw(19) << v->getInfo() << "| " << setw(13) << before << "  | " << setw(13) << delafter << " | " << setw(13) << difference << "|" << endl;
        }
    }
    cout << "+--------------------+---------------+---------------+---------------+" << endl;


    //resetar o grafo
    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == r){
            for (auto edge : v->getIncoming()){
                edge->setWeight(peso);
            }
        }
    }
}



void System::permaremoveReservoir(const string& r){


    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == r){
            for (auto edge : v->getIncoming()){
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
            //como queremos remover reservatórios um de cada vez, temos atualizar o maxflow de cada cidade a cada remoção
            double difference = delafter - before;
            cout << "| " << left << setw(19) << v->getInfo() << "| " << setw(13) << before << "  | " << setw(13) << delafter << " | " << setw(13) << difference << "|" << endl;
        }
    }
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

}


//esta função é a responsável por remover vários reservatórios
void System::removeReservoirVector(const vector<string>& vetor) {

    //guardamos os mapas para nao perdermos a informação ao atualizar os maxflows
    auto map2 = codeToReservoir;
    auto map3 = codeToCity;

    //vetor para guardar os reservatórios removidos previamente
    //o par vai ter o código do reservatório e a sua delivery máxima
    //vai ser usado mais tarde para recuperar a informação perdida ao remover os reservatórios
    vector<pair<string,double>> removed;

    for (const auto& r : vetor){
        //verifica se existe o reservatório
        if (codeToReservoir.find(r) != codeToReservoir.end()){
            //chama a função de remover permanentemente
            //lembrar que isto vai atualizar o maxflow de cada cidade
            permaremoveReservoir(r);
            codeToReservoir = map2;

            //adicionar o reservatorio ao vetor de removidos
            removed.emplace_back(r, codeToReservoir.at(r).getDelivery());
        }
        else{
            //printa isto se o reservatorio nao existir e salta para o proximo input do vector
            cout << "The chosen reservoir, '" << r << "'does not exist" << endl;
            cout << "Skipping..." << endl << endl;
        }

    }

    codeToCity = map3; //faz-se isto para resetar os maxflows, visto que as permaremove functions atualizam os maxflows das cidades a cada remoção

    for (auto v : graph.getVertexSet()){
        //para cada vertex
        if (v->getInfo()[0] == 'r'){
            //se for a SuperSource ('r' é para a Supersource, 'R' para reservatorios normais)
            for (auto e : v->getAdj()){
                //para cada edge
                for (auto& p : removed){
                    //para cada par no vetor de removidos
                    if (e->getDest()->getInfo() == p.first){
                        //se o destino da edge for igual ao código do reservatório
                        e->setWeight(p.second); //definir o peso da edge para o peso que tinha anteriormente
                        break; //sair do loop porque só temos de mexer numa edge por reservatório e já a encontramos
                    }
                }
            }
            break; //sair do loop principal porque só temos de mexer na Supersource, ela está ligada a todos os reservatorios
        }
    }

}



//mesma coisa que o removeReservoir mas para as Stations
//remove apenas uma e volta a por no sitio
void System::removePS(const string& ps){


    //verificar se a estação existe
    if (codeToStation.find(ps) == codeToStation.end()){
        cout << "The chosen Station, '" << ps << "'does not exist" << endl;
        cout << "Please try again with valid input." << endl;
        return;
    }

    //impedir que chegue lá flow
    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == ps){
            for (auto e : v->getIncoming()){
                e->setWeight(0);
            }
            break;
        }
    }

    //atualizar flows
    edmondsKarp(graph,"r_Super","c_Super");


    //printing muito semelhante ao anterior
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


    //resetar o grafo
    for (auto v : graph.getVertexSet()){
        if (v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
            for (auto edge : v->getAdj()){
                //se não for a supersource ou a super target, resetar
                //a supersource e a supertarget nao fazem parte dos maps então ia dar erros ao tentar acedê-las
                string source = edge->getOrig()->getInfo();
                string target = edge->getDest()->getInfo();
                edge->setFlow(codesToPipe.at(source+target).getFlow());
                edge->setWeight(codesToPipe.at(source+target).getCapacity());
            }
        }
    }
}

//mesmo conceito que para reservatorios
//esta vai ser a auxiliar de remoção de vector com códigos de pumping stations
void System::permaremovePS(const string& ps){


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
            codeToCity.at(v->getInfo()).setMaxFlow(delafter); //atualizar maxflows
            double difference = delafter - before;
            cout << "| " << left << setw(19) << v->getInfo() << "| " << setw(13) << before << "  | " << setw(13) << delafter << " | " << setw(13) << difference << "|" << endl;
        }
    }
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

}

void System::removePSVector(const vector<string>& vetor) {

    //guardar a informação
    auto map3 = codeToCity;


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

    codeToCity = map3; //faz-se isto para resetar os maxflows, visto que as permaremove functions atualizam os maxflows das cidades a cada remoção

    for (auto v : graph.getVertexSet()){
        if (v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
            for (auto edge : v->getAdj()){
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


    if (codesToPipe.at(pa+pb).getDirected() == 1){
        for (auto v : graph.getVertexSet()){
            if (v->getInfo() == pa){
                for (auto edge : v->getAdj()){
                    if (edge->getDest()->getInfo() == pb){
                        edge->setWeight(0);
                        break;
                    }
                }
                break;
            }
        }
    }
    else{
        for (auto v : graph.getVertexSet()){
            if (v->getInfo() == pa){
                for (auto edge : v->getAdj()){
                    if (edge->getDest()->getInfo() == pb){
                        edge->setWeight(0);
                        break;
                    }
                }
            }
            if (v->getInfo() == pb){
                for (auto edge : v->getAdj()){
                    if (edge->getDest()->getInfo() == pa){
                        edge->setWeight(0);
                        break;
                    }
                }
            }
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
        if (v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
            for (auto e : v->getAdj()){
                string source = e->getOrig()->getInfo();
                string target = e->getDest()->getInfo();
                e->setFlow(codesToPipe.at(source+target).getFlow());
                e->setWeight(codesToPipe.at(source+target).getCapacity());
            }
        }
    }
}

void System::permaremovePipe(const string& pa, const string& pb){


    if (codesToPipe.at(pa+pb).getDirected() == 1){
        for (auto v : graph.getVertexSet()){
            if (v->getInfo() == pa){
                for (auto edge : v->getAdj()){
                    if (edge->getDest()->getInfo() == pb){
                        edge->setWeight(0);
                        break;
                    }
                }
                break;
            }
        }
    }
    else{
        for (auto v : graph.getVertexSet()){
            if (v->getInfo() == pa){
                for (auto edge : v->getAdj()){
                    if (edge->getDest()->getInfo() == pb){
                        edge->setWeight(0);
                        break;
                    }
                }
            }
            if (v->getInfo() == pb){
                for (auto edge : v->getAdj()){
                    if (edge->getDest()->getInfo() == pa){
                        edge->setWeight(0);
                        break;
                    }
                }
            }
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

    auto map2 = codeToReservoir;
    auto map3 = codeToCity;


    for (const auto& pipe : vetor){
        if (codesToPipe.find(pipe) != codesToPipe.end()){
            if (codesToPipe.at(pipe).getSource() != "r_Super" && codesToPipe.at(pipe).getTarget() != "c_Super"){
            permaremovePipe(codesToPipe.at(pipe).getSource(),codesToPipe.at(pipe).getTarget()); //remover
            }
        }
        else{
            cout << endl;
            cout << "The next Pipeline in queue does not exist" << endl;
            cout << "Skipping..." << endl << endl;
        }
    }


    //resetar a informação

    codeToCity = map3;

    for (auto v : graph.getVertexSet()){
        if(v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
            for (auto edge : v->getAdj()){
                string source = edge->getOrig()->getInfo();
                string target = edge->getDest()->getInfo();
                edge->setFlow(codesToPipe.at(source+target).getFlow());
                edge->setWeight(codesToPipe.at(source+target).getCapacity());
            }
        }
    }
}
