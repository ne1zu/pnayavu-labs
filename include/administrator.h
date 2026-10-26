#pragma once
#include <string>
#include <string_view>
#include <iostream>
#include "person.h"

class Administrator : public Person {
private:
    std::string position;
    int experience;
    int patientsPerDay;
    static int counter;

public:
    Administrator(std::string_view name = "", std::string_view position = "",
        int experience = 0, int patientsPerDay = 0);
    Administrator(int id, std::string_view name, std::string_view position,
        int experience, int patientsPerDay);

    std::string getPosition() const;
    int getExperience() const;
    int getPatientsPerDay() const;

    void setPosition(std::string_view positionValue);
    void setExperience(int expValue);
    void setPatientsPerDay(int countValue);

    bool operator==(const Administrator& other) const;
    bool operator<(const Administrator& other) const;
    bool operator>(const Administrator& other) const;

    std::string getEntityType() const override;
    void printInfo(std::ostream& os) const override;
    std::string classify() const override;

    friend std::istream& operator>>(std::istream& is, Administrator& admin);
};