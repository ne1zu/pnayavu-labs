#pragma once
#include <string>
#include "clinic_exception.h"

class FileIOException : public ClinicException {
public:
    FileIOException(const std::string& filename, const std::string& operation)
        : ClinicException("Error while " + operation + " file '" + filename + "'.") {}
};
