#pragma once
#include <string>
#include <string_view>
#include <iostream>
#include "clinic_record.h"

class Patient : public ClinicRecord {
private:
    int age;
    std::string phone;

    static int counter;

public:
    Patient(std::string_view fio = "", int age = 0, std::string_view phone = "");

    std::string getFio() const;
    int getAge() const;
    std::string getPhone() const;

    void setFio(std::string_view fioValue);
    void setAge(int ageValue);
    void setPhone(std::string_view phoneValue);

    bool isAdult() const;

    bool operator==(const Patient& other) const;
    bool operator<(const Patient& other) const;
    bool operator>(const Patient& other) const;

    
    std::string getEntityType() const override;
    void printInfo(std::ostream& os) const override;
    std::string classify() const override;

    friend std::istream& operator>>(std::istream& is, Patient& patient);
};
