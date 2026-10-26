#pragma once
#include <string>
#include "clinic_exception.h"

class FileFormatException : public ClinicException {
private:
    int line;

public:
    FileFormatException(int line, const std::string& message)
        : ClinicException("Wrong data in line " + std::to_string(line) + ": " + message), line(line) {}

    int getLine() const { return line; }
};
