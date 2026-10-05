#pragma once
#include "person.h"

class Administrator : public Person {
private:
    std::string position;
    int managedPeople;
    static int counter;

public:
    Administrator(std::string_view name = "", std::string_view position = "", int managedPeople = 0);

    std::string getPosition() const;
    int getManagedPeople() const;

    void setPosition(std::string_view posValue);
    void setManagedPeople(int count);

    bool operator==(const Administrator& other) const;
    bool operator<(const Administrator& other) const;
    bool operator>(const Administrator& other) const;

    std::string getEntityType() const override;
    void printInfo(std::ostream& os) const override;
    std::string classify() const override;

    friend std::istream& operator>>(std::istream& is, Administrator& admin);
};