#pragma once
#include <string>
#include "clinic_exception.h"

class CapacityExceededException : public ClinicException {
private:
    std::string objectType;
    int capacity;

public:
    CapacityExceededException(const std::string& place, const std::string& objectType, int capacity)
        : ClinicException(place + ": capacity for " + objectType + " is full (limit "
            + std::to_string(capacity) + ")."),
        objectType(objectType), capacity(capacity) {}

    const std::string& getObjectType() const { return objectType; }
    int getCapacity() const { return capacity; }
};
