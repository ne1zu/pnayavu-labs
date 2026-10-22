#pragma once
#include <string>
#include "clinic_exception.h"

class InvalidOperationException : public ClinicException {
private:
    std::string operation;

public:
    InvalidOperationException(const std::string& place, const std::string& operation,
        const std::string& reason)
        : ClinicException(place + ": cannot " + operation + " - " + reason + "."),
        operation(operation) {}

    const std::string& getOperation() const { return operation; }
};
