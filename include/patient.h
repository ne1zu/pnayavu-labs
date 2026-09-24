#pragma once
#include <string>
#include <string_view>
#include <iostream>

class Patient {
private:
    int id;
    std::string fio;
    int age;
    std::string phone;

    static int counter;

public:
    Patient(std::string_view fio = "", int age = 0, std::string_view phone = "");

    int getId() const;
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

    friend std::ostream& operator<<(std::ostream& os, const Patient& patient);
    friend std::istream& operator>>(std::istream& is, Patient& patient);
};
