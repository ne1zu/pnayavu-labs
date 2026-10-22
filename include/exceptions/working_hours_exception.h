#pragma once
#include <string>
#include "clinic_exception.h"

class WorkingHoursException : public ClinicException {
private:
    std::string workingHours;

public:
    WorkingHoursException(const std::string& appointmentTime, const std::string& workingHours)
        : ClinicException("Appointment " + appointmentTime + " does not fit into working hours "
            + workingHours + " (service length included)."),
        workingHours(workingHours) {}

    const std::string& getWorkingHours() const { return workingHours; }
};
