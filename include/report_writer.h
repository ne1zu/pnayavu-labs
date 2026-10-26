#pragma once
#include <string>
#include "clinic_data.h"

class ReportWriter {
private:
    std::string filename;

public:
    explicit ReportWriter(const std::string& filename);

    void write(const ClinicData& data) const;
};
