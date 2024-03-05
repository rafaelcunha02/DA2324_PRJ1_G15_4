#include "reader.h"

reader::reader() = default;

void reader::readAndParseNodes() {

    ifstream file ("..data/Reservoir.csv");
    string line;

    getline(file, line);
    while (getline(file, line)){
        stringstream ss(line);
        string reservoir, municipality, id, code, del;
        int delivery;

        WaterReservoir()

    }

}
