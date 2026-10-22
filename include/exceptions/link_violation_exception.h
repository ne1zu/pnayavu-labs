#pragma once
#include <string>
#include "clinic_exception.h"

class LinkViolationException : public ClinicException {
public:
    LinkViolationException(const std::string& place, const std::string& reason)
        : ClinicException(place + ": " + reason + ".") {}
};
