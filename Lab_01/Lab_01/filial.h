#pragma once
#include <string>
#include "doctor.h"
#include "service.h"

class Filial {
private:
    std::string name;
    std::string address;

    Doctor** doctors;
    int doctorCount;
    int doctorCapacity;

    Service** services;
    int serviceCount;
    int serviceCapacity;

public:
    Filial(const std::string& name, const std::string& address, int doctorCapacity, int serviceCapacity);
    ~Filial();

    Filial(const Filial& other);
    Filial& operator=(const Filial& other);

    bool addDoctor(Doctor* doctor);
    bool addService(Service* service);
    bool isServiceAvailable(int serviceId) const;

    std::string getName() const;
    std::string getAddress() const;
    int getDoctorCount() const;
    int getServiceCount() const;

    void printInfo() const;
};