#pragma once
#include <string>
#include <string_view>

class Doctor {
private:
    int id;
    std::string fio;
    std::string specialty;
    int experience;

    static int counter;

public:
    Doctor(std::string_view fio, std::string_view specialty, int experience);

    int getId() const;
    std::string getFio() const;
    std::string getSpecialty() const;
    int getExperience() const;

    void setFio(std::string_view fioValue);
    void setSpecialty(std::string_view specialtyValue);
    void setExperience(int experience);

    void printInfo() const;
};