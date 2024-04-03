#ifndef DA2324_PRJ1_G15_4_WATER_RESERVOIR_H
#define DA2324_PRJ1_G15_4_WATER_RESERVOIR_H

#include <iostream>
#include <string>

/**
 * @brief Class for reservoirs.
 * @details Stores the information about a single reservoir.
 */
class Reservoir {

private:
    /// @brief The reservoir's name.
    std::string name;
    /// @brief The reservoir's municipality.
    std::string municipality;
    /// @brief The reservoir's id.
    int id;
    /// @brief The reservoir's code, represented as a string 'R_' followed by the id.
    std::string code;
    /// @brief The reservoir's delivery.
    int delivery;

public:
    /*!
     * @brief Constructor of the Reservoir class.
     * @details Initializes the  member values with the given ones.
     * @note Time complexity: O(1).
     */
    Reservoir(const std::string& name, const std::string& municipality, int id, std::string code, int delivery) :
            name(name), municipality(municipality), id(id), code(code), delivery(delivery) {}


    /*!
     * @brief Getter for the reservoir's name.
     * @returns String with the reservoir's name.
     */
    std::string getName() const { return name; }

    /*!
     * @brief Getter for the reservoir's municipality.
     * @returns String with the reservoir's municipality.
     */
    std::string getMunicipality() const { return municipality; }

    /*!
     * @brief Getter for the reservoir's id.
     * @returns Integer with the reservoir's id.
     */
    int getId() const { return id; }

    /*!
     * @brief Getter for the reservoir's code.
     * @returns String with the reservoir's code.
     */
    std::string getCode() const { return code; }

    /*!
     * @brief Getter for the reservoir's delivery.
     * @returns Integer with the reservoir's delivery.
     */
    int getDelivery() const { return delivery; }


    void setDelivery(int delivery){
        this->delivery = delivery;
    }


    /*!
     * @brief Overloading of operator== used by unordered_set for checking equality between two Reservoir objects.
     * @param other Reservoir object for comparison.
     * @return True if the reservoirs' ids, names and municipalities are equal, false otherwise.
     */
    bool operator==(const Reservoir& other) const {
        return id == other.getId() && name == other.getName() && municipality == other.getMunicipality();
    }
};

#endif // DA2324_PRJ1_G15_4_WATER_RESERVOIR_H
