#pragma once
#include <string>
#include "invalid_data_exception.h"

class InvalidDateTimeException : public InvalidDataException {
public:
    explicit InvalidDateTimeException(const std::string& reason)
        : InvalidDataException("date/time", reason) {}
};
