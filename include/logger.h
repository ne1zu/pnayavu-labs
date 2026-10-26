#pragma once
#include <string>

class Logger {
private:
    std::string filename;
    mutable bool failureReported;

public:
    explicit Logger(const std::string& filename);

    void log(const std::string& event, const std::string& details = "") const;
};
