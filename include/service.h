#pragma once
#include <string>
#include <string_view>
#include <iostream>


class Service {
private:
    int id;
    std::string name;
    float price;
    int length;

    static int counter;

public:
    Service(std::string_view name = "", float price = 0.0f, int length = 0);

    int getId() const;
    std::string getName() const;
    float getPrice() const;
    int getLength() const;

    void setName(std::string_view nameValue);
    void setPrice(float priceValue);
    void setLength(int lengthValue);

    bool operator==(const Service& other) const;
    bool operator<(const Service& other) const;
    bool operator>(const Service& other) const;

    std::string getEntityType() const;
    void printInfo(std::ostream& os) const;
    std::string classify() const;

    friend std::ostream& operator<<(std::ostream& os, const Service& srv);
    friend std::istream& operator>>(std::istream& is, Service& srv);
};
