#ifndef UNTITLED17_PIPE_H
#define UNTITLED17_PIPE_H

#include <string>

/**
 * @brief Class for pipes.
 * @details Stores the information about a single pipe.
 */
class Pipe {

private:
    /// @brief The pipe's source represented as a string with the code for either a reservoir or a pumping station.
    std::string source;
    /// @brief The pipe's target represented as a string with the code for either a city or a pumping station.
    std::string target;
    /// @brief The pipe's capacity.
    int capacity;
    /// @brief The pipe's flow.
    int flow;

public:
    /*!
     * @brief Constructor of the Pipe class.
     * @details Initializes the  member values with the given ones.
     * @note Time complexity: O(1).
     */
    Pipe(const std::string& source, const std::string& target, int capacity, int flow) :
            source(source), target(target), capacity(capacity), flow(flow) {}


    /*!
     * @brief Getter for the pipe's source.
     * @returns String with the pipe's source.
     */
    std::string getSource() const { return source; }

    /*!
     * @brief Getter for the pipe's target.
     * @returns String with the pipe's target.
     */
    std::string getTarget() const { return target; }

    /*!
     * @brief Getter for the pipe's capacity.
     * @returns Integer with the pipe's capacity.
     */
    int getCapacity() const { return capacity; }

    /*!
     * @brief Getter for the pipe's flow.
     * @returns Integer with the pipe's flow.
     */
    int getFlow() const { return flow; }


    /*!
     * @brief Overloading of operator== used by unordered_set for checking equality between two Pipe objects.
     * @param other Pipe object for comparison.
     * @return True if the pipes' sources, targets and capacities are equal, false otherwise.
     */
    bool operator==(const Pipe& other) const {
        return source == other.getSource() && target == other.getTarget() && capacity == other.getCapacity();
    }
};

#endif //UNTITLED17_PIPE_H
