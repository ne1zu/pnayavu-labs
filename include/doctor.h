#pragma once
#include <string>
#include <string_view>
#include <iostream>
#include "clinic_record.h"

class Doctor : public ClinicRecord {
private:
    std::string specialty;
    int experience;

    static int counter;

public:
    Doctor(std::string_view fio = "", std::string_view specialty = "", int experience = 0);

    std::string getFio() const;
    std::string getSpecialty() const;
    int getExperience() const;

    void setFio(std::string_view fioValue);
    void setSpecialty(std::string_view specialtyValue);
    void setExperience(int expValue);

    bool operator==(const Doctor& other) const;
    bool operator<(const Doctor& other) const;
    bool operator>(const Doctor& other) const;

    
    std::string getEntityType() const override;
    void printInfo(std::ostream& os) const override;
    std::string classify() const override;

    friend std::istream& operator>>(std::istream& is, Doctor& doc);
};
