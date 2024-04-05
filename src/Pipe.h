#ifndef DA2324_PRJ1_G15_4_PIPE_H
#define DA2324_PRJ1_G15_4_PIPE_H

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
    /// @brief The pipe's directedness or undirectedness.
    int directed;

public:

    /*!
     * @brief Constructor of the Pipe class.
     * @details Initializes the  member values with the given ones.
     * @note Time complexity: O(1).
     */
    Pipe(const std::string& source, const std::string& target, int capacity, int flow, int directed) :
            source(source), target(target), capacity(capacity), flow(flow), directed(directed) {}


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
     * @brief Getter for the pipe's directedness.
     * @returns Integer with the pipe's directedness.
     */
    int getDirected() const { return directed; }



    /*!
     * @brief Setter for the pipe's flow.
     * @param f Double with the new flow value.
     */
    void setFlow(double f) {
        flow = f;
    }

    /*!
     * @brief Overloading of operator== used by unordered_set for checking equality between two Pipe objects.
     * @param other Pipe object for comparison.
     * @return True if the pipes' sources, targets and capacities are equal, false otherwise.
     */
    bool operator==(const Pipe& other) const {
        return source == other.getSource() && target == other.getTarget() && capacity == other.getCapacity();
    }
};

#endif //DA2324_PRJ1_G15_4_PIPE_H
