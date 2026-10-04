#pragma once
#include <string>
#include <string_view>
#include <iostream>
#include "clinic_record.h"

class Service : public ClinicRecord {
private:
    float price;
    int length;

    static int counter;

public:
    Service(std::string_view name = "", float price = 0.0f, int length = 0);

    float getPrice() const;
    int getLength() const;

    void setPrice(float priceValue);
    void setLength(int lengthValue);

    bool operator==(const Service& other) const;
    bool operator<(const Service& other) const;
    bool operator>(const Service& other) const;

    
    std::string getEntityType() const override;
    void printInfo(std::ostream& os) const override;
    std::string classify() const override;

    friend std::istream& operator>>(std::istream& is, Service& srv);
};
