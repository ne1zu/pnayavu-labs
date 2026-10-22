#pragma once
#include <string>
#include "clinic_exception.h"

class ScheduleConflictException : public ClinicException {
private:
    std::string personType;
    std::string personName;

public:
    ScheduleConflictException(const std::string& personType, const std::string& personName,
        const std::string& appointmentTime)
        : ClinicException(personType + " '" + personName + "' already has an appointment at this time ("
            + appointmentTime + ")."),
        personType(personType), personName(personName) {}

    const std::string& getPersonType() const { return personType; }
    const std::string& getPersonName() const { return personName; }
};
