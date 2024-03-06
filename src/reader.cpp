#include "reader.h"
#include <string>


using namespace std;

reader::reader() = default;

void reader::readAndParseNodes() {

    ifstream file(R"(C:\Users\Rafa\CLionProjects\DA2324_PRJ1_G15_4\data\Reservoir.csv)");
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

        try {
            delivery = stoi(del);
            id = stoi(ident);
        } catch (const std::invalid_argument& e) {
            // Handle the invalid argument error here
            std::cerr << "Invalid argument: " << e.what() << std::endl;
            // Optionally, you can skip processing this line or take other actions
            continue; // Skip processing this line and move to the next one
        }

        WaterReservoir WR(name, municipality, id, code, delivery);
        codeToReservoir.insert({code, WR});

        cout << "Map size: " << codeToReservoir.size() << endl;
        graph.addVertex(WR.getCode());

    }

    //---------------------------------------//

    ifstream file2(R"(C:\Users\Rafa\CLionProjects\DA2324_PRJ1_G15_4\data\Cities.csv)");
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

    ifstream file3(R"(C:\Users\Rafa\CLionProjects\DA2324_PRJ1_G15_4\data\Stations.csv)");
    string line3;

    getline(file3, line3);
    while (getline(file3, line3)){
        stringstream ss(line3);
        string Ident, Code;
        int id;
        getline(ss, Ident, ',');
        getline(ss, Code, ',');

        id = stoi(Ident);


        PumpingStation PS(id,Code);
        codeToStation.insert({Code, PS});

        graph.addVertex(PS.getCode());

    }

}


void reader::readAndParseEdges() {

    ifstream file(R"(C:\Users\Rafa\CLionProjects\DA2324_PRJ1_G15_4\data\Pipes.csv)");
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
