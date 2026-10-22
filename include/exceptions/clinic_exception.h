#pragma once
#include <stdexcept>
#include <string>

class ClinicException : public std::runtime_error {
public:
    explicit ClinicException(const std::string& message) : std::runtime_error(message) {}
};
