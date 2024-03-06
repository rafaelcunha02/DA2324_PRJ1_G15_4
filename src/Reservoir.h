#ifndef DA2324_PRJ1_G15_4_WATER_RESERVOIR_H
#define DA2324_PRJ1_G15_4_WATER_RESERVOIR_H

#include <iostream>
#include <string>

class Reservoir {

public:
    Reservoir(const std::string& name, const std::string& municipality, int id, std::string code, int delivery) :
            name(name), municipality(municipality), id(id), code(code), delivery(delivery) {}

    std::string getName() const { return name; }
    std::string getMunicipality() const { return municipality; }
    int getId() const { return id; }
    std::string getCode() const { return code; }
    int getDelivery() const { return delivery; }

    bool operator==(const Reservoir& other) const {
        // Compare relevant attributes for equality
        return getId() == other.getId() && name == other.name && municipality == other.municipality;
    }

private:
    std::string name;
    std::string municipality;
    int id;
    std::string code;
    int delivery;
};

#endif // DA2324_PRJ1_G15_4_WATER_RESERVOIR_H
