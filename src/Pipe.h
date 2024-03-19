//
// Created by Rafa on 19/03/2024.
//

#ifndef UNTITLED17_PIPE_H
#define UNTITLED17_PIPE_H


#include <string>

class Pipe {

private:
    std::string source;
    std::string target;
    int capacity;
    int flow;

public:
    Pipe(const std::string& source, const std::string& target, int capacity, int flow) :
            source(source), target(target), capacity(capacity), flow(flow) {}

    std::string getSource() const { return source; }
    std::string getTarget() const { return target; }
    int getCapacity() const { return capacity; }
    int getFlow() const { return flow; }

    bool operator==(const Pipe& other) const {
        return getSource() == other.getSource() && target == other.target && capacity == other.capacity;
    }

};


#endif //UNTITLED17_PIPE_H
