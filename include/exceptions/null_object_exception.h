#pragma once
#include <string>
#include "clinic_exception.h"

class NullObjectException : public ClinicException {
private:
    std::string objectType;

public:
    NullObjectException(const std::string& place, const std::string& objectType)
        : ClinicException(place + ": " + objectType + " is null."), objectType(objectType) {}

    const std::string& getObjectType() const { return objectType; }
};
