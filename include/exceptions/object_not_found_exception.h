#pragma once
#include <string>
#include "clinic_exception.h"

class ObjectNotFoundException : public ClinicException {
private:
    std::string objectType;

public:
    ObjectNotFoundException(const std::string& place, const std::string& objectType)
        : ClinicException(place + ": this " + objectType + " is not found."), objectType(objectType) {}

    const std::string& getObjectType() const { return objectType; }
};
