#pragma once
#include <string>
#include "clinic_exception.h"

class FileOpenException : public ClinicException {
public:
    FileOpenException(const std::string& filename, const std::string& purpose)
        : ClinicException("Cannot open file '" + filename + "' for " + purpose + ". Check the path and the access rights.") {}
};
