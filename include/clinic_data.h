#pragma once
#include <memory>
#include <vector>
#include "service.h"
#include "doctor.h"
#include "patient.h"
#include "administrator.h"
#include "filial.h"

struct ClinicData {
    std::vector<std::shared_ptr<Service>> services;
    std::vector<std::shared_ptr<Doctor>> doctors;
    std::vector<std::shared_ptr<Patient>> patients;
    std::vector<std::shared_ptr<Administrator>> administrators;
    std::vector<std::shared_ptr<Filial>> filials;
};
