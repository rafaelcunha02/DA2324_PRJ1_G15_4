#ifndef DA2324_PRJ1_G15_4_CITY_H
#define DA2324_PRJ1_G15_4_CITY_H

#include <iostream>
#include <string>

class City {

public:
    City(const std::string& name, int id, const std::string& code, int demand, int population) :
            name(name), id(id), code(code), demand(demand), population(population) {}

    std::string getName() const { return name; }
    int getId() const { return id; }
    std::string getCode() const { return code; }
    int getDemand() const { return demand; }
    int getPopulation() const { return population; }

    bool operator==(const City& other) const {
        return getId() == other.getId() && name == other.name && code == other.code;
    }

private:
    std::string name;
    int id;
    std::string code;
    int demand;
    int population;
};

#endif // DA2324_PRJ1_G15_4_CITY_H
