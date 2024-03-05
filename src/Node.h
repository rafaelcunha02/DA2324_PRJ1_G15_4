//
// Created by Rafa on 05/03/2024.
//

#ifndef DA2324_PRJ1_G15_4_NODE_H
#define DA2324_PRJ1_G15_4_NODE_H


#include <iostream>
#include <string>

class Node {
protected:
    int id;
    std::string location;

public:
    Node(int id, const std::string& location) : id(id), location(location) {}

    virtual void printDetails() const {
        std::cout << "Node ID: " << id << ", Location: " << location << std::endl;
    }

    virtual int getId() const { return id; }
    virtual std::string getLocation() const { return location; }
};

class WaterReservoir : public Node {
protected:
    int capacity;

public:
    WaterReservoir(int id, const std::string& location, int capacity) :
            Node(id, location), capacity(capacity) {}

    void printDetails() const override {
        Node::printDetails();
        std::cout << "Capacity: " << capacity << " m³/sec" << std::endl;
    }

    int getCapacity() const { return capacity; }
};

class PumpingStation : public Node {
protected:
    int power;

public:

    PumpingStation(int id, const std::string& location, int power) :
            Node(id, location), power(power) {}

    void printDetails() const override {
        Node::printDetails();
        std::cout << "Power: " << power << " kW" << std::endl;
    }

    int getPower() const { return power; }
};

class DeliverySite : public Node {
public:
    int demand;

    DeliverySite(int id, const std::string& location, int demand) :
            Node(id, location), demand(demand) {}

    void printDetails() const override {
        Node::printDetails();
        std::cout << "Demand: " << demand << " m³/sec" << std::endl;
    }

    int getDemand() const { return demand; }
};


#endif //DA2324_PRJ1_G15_4_NODE_H
