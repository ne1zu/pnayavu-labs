#pragma once
#include <string>
#include "clinic_exception.h"

class InvalidDataException : public ClinicException {
private:
    std::string field;

public:
    InvalidDataException(const std::string& field, const std::string& reason)
        : ClinicException("Invalid value for '" + field + "': " + reason + "."), field(field) {}

    const std::string& getField() const { return field; }
};
