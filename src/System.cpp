#include "System.h"
#include <string>
#include <iomanip>


using namespace std;


/**
 * @brief Default constructor
 */
System::System() = default;

//lê os ficheiros e cria os objetos


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

        //cria reservatorio, adiciona ao grafo de strings, associa a string à informação num mapa
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
    //tambem temos mapa de Pipes, mas nao adiciona já ao mapa e já te explico porquê
    //Vê a função void System::initialize() para perceberes o porquê

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


//é suposto chamarmos esta função no inicio de tudo
//esta função vai criar as Super source e Super Target
//vai correr pela 1a vez o EdmondsKarp
//vai preencher finalmente o Mapa das Pipes
//esse mapa de Pipes vai associar o código da source concatenado com o codigo da target à informação da Pipe
//O readAndParseEdges nao adiciona logo as Pipes a esse mapa, porque só depois de correr o EdmondsKarp é que elas vao ter flow
//e dá jeito ter o flow guardado para as outras funções
//porque nao so vamos ter de resetar o grafo
//como vai facilitar a procura de informação, em vez de procurar no grafo, vamos só sacar do mapa diretamente
//sacar do mapa se n me engano, complexidade = O(1)
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
                cout << identifier << endl;
                codesToPipe.at(identifier).setFlow(e->getFlow()); //associar o flow à informação
            }
        }
    }

    //isto preenche um ficheiro como eles mandam fazer, com o maxflow inicial de cada cidade
    //sem ter em conta remoções, etc
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
//repara que isto só vai simplesmente buscar o maxflow que já foi calculado no initialize()
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

//mesma coisa que o anterior ,as para todas as cidades uma de cada vez

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


//printa o somatório de todos os max flows
//acho q isto tá uma beca feio, se puderes por mais Pipi era fixe
void System::maxFlowSystem(){
    double tot = 0;
    for (const auto& par : codeToCity){
        tot += par.second.getMaxFlow();
    }

    cout << "The maximum flow for this network is " << tot << "m^3/sec";
}


//compara o maxflow de cada cidade com a sua demanda e printa o quanto falta caso nao chegue à demanda
//water deficit = demanda - maxflow
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



//a VARIANCIA é calculada aqui dentro
//é aqui que supostamente tá mal
//isto também printa e tá um bocado feio, era fixe alterar para ser mais user friendly
void System::averageFlowPipes(){

    double total = 0; //somatório do flow de todas as edges
    double totalmedias = 0; //somatório das médias de flow de todas as edges
    double count = 0; //contador de edges

    for (auto v : graph.getVertexSet()){
        for (auto e : v->getAdj()){
            //para cada edge
            auto source = e->getOrig()->getInfo();
            auto target = e->getDest()->getInfo();

            //printa de onde vai até onde, o flow atual, o peso e a percentagem de flow;
            cout << "Pipe from " << source << " to " << target << ": "
            << e->getFlow() << "/" << e->getWeight()
            << " (" << (e->getFlow()/e->getWeight()) * 100 << "%)" << endl;


            count++; //incrementa o contador
            total += e->getFlow(); //adiciona o flow ao total
            totalmedias += (e->getFlow()/e->getWeight()) * 100; //adiciona a percentagem de flow ao total de médias

        }
    }

    cout << "---------------------";
    cout << "Final stats--------------------" << endl;
    cout << "Average flow per pipe: " << total/count << endl; //media de flow
    cout << "Average percentage of flow per pipe: " << totalmedias / count << endl; //média das médias de flow


    //Calculo da Variancia
    //literalmente segui pseudocodigo da wikipedia e nem assim, alguem me mate bruh
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

    //mais um print feio
    cout << "Variance: " << variance;

}


//função que remove um reservatório e reseta o grafo logo a seguir
//usada para remover um unico reservatório (ver as 3 opções do Menu)
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
                //isto faz com que a operação seja kinda rápida
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
            double before = codeToCity.at(v->getInfo()).getMaxFlow(); //no Initialize(), setamos o maxflow de cada cidade para o maxflow default do dataset
            double difference = delafter - before;
            cout << "| " << left << setw(19) << v->getInfo() << "| " << setw(13) << before << "  | " << setw(13) << delafter << " | " << setw(13) << difference << "|" << endl;
        }
    }
    cout << "+--------------------+---------------+---------------+---------------+" << endl;


    //resetar o grafo
    //neste caso é facil porque é só resetar o peso da edge que removemos
    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == r){
            for (auto edge : v->getIncoming()){
                edge->setWeight(peso);
            }
        }
    }
}



//função anterior, ligeiramente alterada para nao resetar o grafo
//existe uma diferença essencial na parte dos maxflows das cidades (ver a parte do printing)
//esta é uma função auxiliar para a função que remove vários reservatórios, chamada removeReservoirVector
//ao fim desta função vem a que remove permanentemente, vais perceber melhor esta quando perceberes essa
//mas lê esta primeiro na mesma
void System::permaremoveReservoir(const string& r){


    //faz o mesmo que a anterior mas nao precisa de guardar o peso porque nao vai resetar já
    for (auto v : graph.getVertexSet()){
        if (v->getInfo() == r){
            for (auto edge : v->getIncoming()){
                edge->setWeight(0);
            }
        }
    }

    //faz o mesmo
    edmondsKarp(graph,"r_Super","c_Super");


    //continua a seguir o mesmo protocolo
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
            codeToCity.at(v->getInfo()).setMaxFlow(delafter); //a grande diferença está aqui
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
    auto map2 = codeToReservoir; //acho que entretanto esta linha ficou desnecessaria agr q vejo bem
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
            codeToReservoir = map2; //eu acho que ja nem precisamos desta linha porque isto foi antes de eu otimizar a situação

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

    //reset the graph using the "removed" vector of pairs to reset the weights
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
                        e->setWeight(p.second); //setar o peso da edge para o peso que tinha anteriormente
                        break; //sair do loop porque só temos de mexer numa edge por reservatório e já a encontramos
                    }
                }
            }
            break; //sair do loop principal porque só temos de mexer na Supersource, ela tá ligada a todos os reservatorios
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
    //isto reseta flows e Weights de todas as edges
    //visto que temos de resetar o flow de todas de qualquer das formas,
    //nao vale a pena ter ifs constantemente a verificarem se elas vão para a estação que removemos
    for (auto v : graph.getVertexSet()){
        for (auto edge : v->getAdj()){
            if (v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
                //se não for a supersource ou a super target, resetar
                //a supersource e a supertarget nao fazem parte dos maps então ia dar erros ao tentar acedê-las
                string source = edge->getOrig()->getInfo();
                string target = edge->getDest()->getInfo();
                edge->setFlow(codesToPipe.at(source+target).getFlow());//acho que esta linha entretanto deixou de ser necessaria
                //testa e tenta ver se dá asneira se a tirares pfv xD

                //se nao der asneira, podemos só resetar a capacity das edges que vao para a estação removida e depois dar logo break
                //isso ia optimizar a situassom
                edge->setWeight(codesToPipe.at(source+target).getCapacity());
            }
        }
    }
}

//mesmo conceito que para reservatorios
//esta vai ser a auxiliar de remoção de vector com códigos de pumping stations
void System::permaremovePS(const string& ps){

    //remover o peso da edges que vao para a station
    //nao sei porquê, mas isto tava a dar pró torto neste caso quando nao removia as Outgoing tambem
    //mas acho que só devia ser necessario remover as Incoming, parece fazer mais sentido
    //até pq na anterior (a q nao remove permanentemente) nao deu asneira remover só as incoming
    //se perceberes e conseguires otimizar sem dar merda nos outputs tás à vontade xD
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

    vector<string> removed; //vetor para dar track ao que ja foi removido
    //entretanto ja nem usamos isto, ao resetar as edges todas dá para descomplicar.
    //No entanto, como a supersource nao faz parte do map, no dos reservatórios tive de ir pela maneira mais complexa um bocado
    //porque visto q elas nao tao no map,
    //por um lado é mais dificil fazer o codigo, mas por outro, a versao dos reservatorios é menos complexa temporalmente pq basta encontrar a supersource ao iterar o grafo e resetar uma só edge


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
        for (auto edge : v->getAdj()){
            if (v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
                //aquele loopzinho classico que ja expliquei em cima que reseta as edges todas
                //todas menos as da supersource e da supertarget porque nao estao nos mapas
                string source = edge->getOrig()->getInfo();
                string target = edge->getDest()->getInfo();
                edge->setFlow(codesToPipe.at(source+target).getFlow());
                edge->setWeight(codesToPipe.at(source+target).getCapacity());
            }
        }
    }
}

//outra vez arroz mas para Pipes, Pipe = Edge
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
        for (auto e : v->getAdj()){
            if (v->getInfo()[0] != 'r' && v->getInfo()[0] != 'c'){
                //loop do costume que (remember) acho que dá pra ser otimizado mas faz cuidado plz
                string source = e->getOrig()->getInfo();
                string target = e->getDest()->getInfo();
                e->setFlow(codesToPipe.at(source+target).getFlow());
                e->setWeight(codesToPipe.at(source+target).getCapacity());
            }
        }
    }
}

//arroz, mas a versão que remove permanentemente
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
            codeToCity.at(v->getInfo()).setMaxFlow(delafter); //arroz que muda o maxflow
            double difference = delafter - before;
            cout << "| " << left << setw(19) << v->getInfo() << "| " << setw(13) << before << "  | " << setw(13) << delafter << " | " << setw(13) << difference << "|" << endl;
        }
    }
    cout << "+--------------------+---------------+---------------+---------------+" << endl;

}


//arroz de vector
void System::removePipeVector(const vector<string>& vetor) {

    auto map2 = codeToReservoir; //guardar a informação (rice)
    //ya esta cena do codetoreservoir decerteza q ja deixou de ser necessaria la em cima
    //eu é que tou com medo de alterar
    //sou um conas

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

    codeToCity = map3; //arroz de resetar maxflows

    //arroz de reset do costume
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

//chegaste ao fim mano és o goat qq coisa manda mensagem eu devo acordar la pas 11 em principio
//desculpa nao ir à aula, espero que te tenha ocupado a maioria das 2h

//agora que mt provavelmente já percebeste as cenas, tenho uma pergunta que me surgiu ao comentar
//achas que o que eles querem é que, ao remover permanentemente, nao resetemos logo o grafo,
//mas que simplesmente dê para fazer todas as operações anteriores com a nova versao do grafo?
//por exemplo, achas que eles querem que, depois de removermos uma cena, ao ir fazer o MaxFlowEachCity,
//ele faça tendo em conta a nova versão do grafo, ou só a original?
//é que se sim, nao era mt complexo de fazer, era mesmo só criar uma função de reset e adaptar um bocado aqui o system
//mas nao sei se é isso que eles querem, se calhar é só para resetar logo

//eu fico na duvida pq cm podes já ter visto, a nossa versão mostra todas as mudanças q acontecem ao remover
//entao nao me pareceu muito necessario fazer isso de nao resetar


//para finalizar faltam fazer os pseudocodigos do 2.3 e do 3.1 tho