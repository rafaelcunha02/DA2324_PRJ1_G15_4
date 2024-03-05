//
// Created by Rafa on 05/03/2024.
//

#ifndef DA2324_PRJ1_G15_4_PIPE_H
#define DA2324_PRJ1_G15_4_PIPE_H


#include <iostream>
#include <string>

class Pipe {
private:
    int id;
    std::string location;
    int capacity;
    bool Directed;


public:

    int getId() const { return id; }
    std::string getLocation() const { return location; }
    int getCapacity() const { return capacity; }
    bool isDirected() const { return Directed; }

    Pipe(int id, const std::string& location, int capacity, bool isDirected) :
            id(id), location(location), capacity(capacity), Directed(isDirected) {}

    void printDetails() const {
        std::cout << "Pipe ID: " << id << ", Location: " << location << std::endl;
        std::cout << "Capacity: " << capacity << " m³/sec" << std::endl;
        std::cout << "Direction: " << (Directed ? "Directed" : "Undirected") << std::endl;
    }
};


#endif //DA2324_PRJ1_G15_4_PIPE_H
