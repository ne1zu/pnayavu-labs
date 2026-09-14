#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <memory> 
#include "doctor.h"
#include "service.h"

class Filial {
private:
    std::string name;
    std::string address;
    std::vector<std::shared_ptr<Doctor>> doctors;
    std::vector<std::shared_ptr<Service>> services;

    int doctorCapacity;
    int serviceCapacity;

public:
    Filial(std::string_view name, std::string_view address, size_t doctorCapacity, size_t serviceCapacity);
    bool addDoctor(std::shared_ptr<Doctor> doctor);
    bool addService(std::shared_ptr<Service> service);
    bool isServiceAvailable(int serviceId) const;

    std::string getName() const;
    std::string getAddress() const;
    int getDoctorCount() const;
    int getServiceCount() const;

    void setName(std::string_view nameValue);
    void setAddress(std::string_view addressValue);

    void printInfo() const;
};