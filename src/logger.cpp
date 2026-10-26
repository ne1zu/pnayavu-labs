#include "logger.h"
#include <fstream>
#include <iostream>
#include "timestamp.h"

namespace {
    std::string makeSingleLine(std::string text) {
        for (char& symbol : text) {
            if (symbol == '\n' || symbol == '\r') symbol = ' ';
        }
        return text;
    }
}

Logger::Logger(const std::string& filename) : filename(filename), failureReported(false) {}

void Logger::log(const std::string& event, const std::string& details) const {
    std::ofstream file(filename, std::ios::app);
    if (file) {
        file << currentTimestamp() << " | " << event;
        if (!details.empty()) file << " | " << makeSingleLine(details);
        file << '\n';
        file.close();
    }

    if (!file && !failureReported) {
        failureReported = true;
        std::cerr << "[WARNING] Cannot write to the journal '" << filename << "'.\n";
    }
}
