#ifndef DA2324_PRJ1_G15_4_NODE_H
#define DA2324_PRJ1_G15_4_NODE_H

#include <iostream>

class Node {
public:
    Node() = default;  // Default constructor for empty class

    virtual void printDetails() const = 0;  // Pure virtual function for printing
};

class WaterReservoir : public Node {
protected:
    string name;
    string municipality;
    int id;
    int capacity;
    int delivery;

public:
    WaterReservoir(const string& name, const string& municipality, int id, int capacity, int delivery) :
    name(name), municipality(municipality), id(id), capacity(capacity), delivery(delivery) {}

    void printDetails() const override {
        std::cout << "Water Reservoir ID: " << id << std::endl;
        std::cout << "Capacity: " << capacity << " m³/sec" << std::endl;
    }

    int getId() const { return id; }
    int getCapacity() const { return capacity; }
};

class PumpingStation : public Node {
protected:
    int id;
    int Code;

public:
    PumpingStation(int id, int code) : id(id), Code(code) {}

    void printDetails() const override {
        std::cout << "Pumping Station ID: " << id << std::endl;
    }

    int getId() const { return id; }
    int getCode() const { return Code; }
};

class DeliverySite : public Node {
protected:
    string name;
    int id;
    int code;
    int demand;
    int population;

public:
    DeliverySite(const string& name, int id, int code, int demand, int population) :
    name(name), id(id), code(code), demand(demand), population(population) {}

    void printDetails() const override {
        std::cout << "Delivery Site ID: " << id << std::endl;
        std::cout << "Demand: " << demand << " m³/sec" << std::endl;
    }

    int getId() const { return id; }
    int getDemand() const { return demand; }
};

#endif //DA2324_PRJ1_G15_4_NODE_H
