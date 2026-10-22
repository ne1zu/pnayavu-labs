#pragma once
#include <string>
#include "clinic_exception.h"

class DuplicateObjectException : public ClinicException {
private:
    std::string objectType;

public:
    DuplicateObjectException(const std::string& place, const std::string& objectType)
        : ClinicException(place + ": this " + objectType + " is already added."), objectType(objectType) {}

    const std::string& getObjectType() const { return objectType; }
};
