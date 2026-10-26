#pragma once
#include <string>
#include "clinic_data.h"

class DataStorage {
private:
    std::string filename;

public:
    explicit DataStorage(const std::string& filename);

    void save(const ClinicData& data) const;
    ClinicData load() const;
};
