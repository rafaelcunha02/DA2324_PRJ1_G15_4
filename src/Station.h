#ifndef DA2324_PRJ1_G15_4_PUMPING_STATION_H
#define DA2324_PRJ1_G15_4_PUMPING_STATION_H

#include <iostream>
#include <string>

/**
 * @brief Class for stations.
 * @details Stores the information about a single station.
 */
class Station {

private:
    /// @brief The station's id.
    int id;
    /// @brief The station's code, represented as a string 'PS_' followed by the id.
    std::string Code;

public:
    /*!
     * @brief Constructor of the Station class.
     * @details Initializes the  member values with the given ones.
     * @note Time complexity: O(1).
     */
    Station(int id, const std::string& code) : id(id), Code(code) {}


    /*!
     * @brief Getter for the station's id.
     * @returns Integer with the station's id.
     */
    int getId() const { return id; }

    /*!
     * @brief Getter for the station's code.
     * @returns String with the station's code.
     */
    const std::string& getCode() const { return Code; }


    /*!
     * @brief Overloading of operator== used by unordered_set for checking equality between two Station objects.
     * @param other Station object for comparison.
     * @return True if the stations' ids and codes are equal, false otherwise.
     */
    bool operator==(const Station& other) const {
        return getId() == other.getId() && Code == other.Code;
    }
};

#endif // DA2324_PRJ1_G15_4_PUMPING_STATION_H
