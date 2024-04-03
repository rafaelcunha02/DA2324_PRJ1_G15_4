#ifndef DA2324_PRJ1_G15_4_CITY_H
#define DA2324_PRJ1_G15_4_CITY_H

#include <iostream>
#include <string>

/**
 * @brief Class for cities.
 * @details Stores the information about a single city.
 */
class City {

private:
    /// @brief The city's full name.
    std::string name;
    /// @brief The city's id, represented as a number.
    int id;
    /// @brief The city's code, represented as a string 'C_' followed by the id.
    std::string code;
    /// @brief The city's demand.
    int demand;
    /// @brief The city's population.
    int population;
    /// @brief The city's max-flow.
    double maxflow;

public:
    /*!
     * @brief Constructor of the City class.
     * @details Initializes the max-flow with the value 0.0 and the other member values with the given ones.
     * @note Time complexity: O(1).
     */
    City(const std::string& name, int id, const std::string& code, int demand, int population) :
            name(name), id(id), code(code), demand(demand), population(population), maxflow(0.0) {}


    /*!
     * @brief Getter for the city's name.
     * @returns String with the city's name.
     */
    std::string getName() const { return name; }

    /*!
     * @brief Getter for the city's id.
     * @returns Integer with the city's id.
     */
    int getId() const { return id; }

    /*!
     * @brief Getter for the city's code.
     * @returns String with the city's code.
     */
    std::string getCode() const { return code; }

    /*!
     * @brief Getter for the city's demand.
     * @returns Integer with the city's demand.
     */
    int getDemand() const { return demand; }

    /*!
     * @brief Getter for the city's population.
     * @returns Integer with the city's population.
     */
    int getPopulation() const { return population; }

    /*!
     * @brief Getter for the city's max-flow.
     * @returns Double with the city's max-flow.
     */
    double getMaxFlow() const { return maxflow; }


    /*!
     * @brief Sets the value of the max-flow member variable.
     * @param maxflow Double containing the new city's max-flow.
     */
    void setMaxFlow(double maxflow) { this->maxflow = maxflow; }


    /*!
     * @brief Overloading of operator== used by unordered_set for checking equality between two City objects.
     * @param other City object for comparison.
     * @return True if the cities' names are equal, false otherwise.
     */
    bool operator==(const City& other) const {
        return getId() == other.getId() && name == other.name && code == other.code;
    }
};

#endif // DA2324_PRJ1_G15_4_CITY_H
