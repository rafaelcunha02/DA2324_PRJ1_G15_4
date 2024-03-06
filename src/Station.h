#ifndef DA2324_PRJ1_G15_4_PUMPING_STATION_H
#define DA2324_PRJ1_G15_4_PUMPING_STATION_H

#include <iostream>
#include <string>

class Station {

public:
    Station(int id, const std::string& code) : id(id), Code(code) {}

    int getId() const { return id; }
    const std::string& getCode() const { return Code; }

    bool operator==(const Station& other) const {
        // Compare relevant attributes for equality
        return getId() == other.getId() && Code == other.Code;
    }

private:
    int id;
    std::string Code;
};

#endif // DA2324_PRJ1_G15_4_PUMPING_STATION_H
