#pragma once
#include <string>
#include <string_view> 

class Service {
private:
    int id;
    std::string name;
    float price;
    int length;

    static int counter;

public:
    Service(std::string_view name, float price, int length);

    int getId() const;
    std::string getName() const;
    float getPrice() const;
    int getLength() const;

    void setName(std::string_view nameValue);
    void setPrice(float priceValue);
    void setLength(int lengthValue);

    void printInfo() const;
};