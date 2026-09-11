#include "C:\Users\user\source\repos\pnayavu-labs\Lab_01\Lab_01\.h\filial.h"
#include <iostream>

Filial::Filial(const std::string& name, const std::string& address, int doctorCapacity, int serviceCapacity)
    : name(name), address(address),
    doctors(new Doctor* [doctorCapacity] {nullptr}), doctorCount(0), doctorCapacity(doctorCapacity),
    services(new Service* [serviceCapacity] {nullptr}), serviceCount(0), serviceCapacity(serviceCapacity)
{
}

Filial::Filial(const Filial& other)
    : name(other.name), address(other.address),
    doctors(new Doctor* [other.doctorCapacity] {nullptr}), doctorCount(other.doctorCount), doctorCapacity(other.doctorCapacity),
    services(new Service* [other.serviceCapacity] {nullptr}), serviceCount(other.serviceCount), serviceCapacity(other.serviceCapacity)
{
    for (int i = 0; i < doctorCount && i < doctorCapacity; i++) {
        doctors[i] = other.doctors[i];
    }
    for (int i = 0; i < serviceCount && i < serviceCapacity; i++) {
        services[i] = other.services[i];
    }
}

Filial& Filial::operator=(const Filial& other) {
    if (this == &other) return *this;

    delete[] doctors;
    delete[] services;

    name = other.name;
    address = other.address;
    doctorCount = other.doctorCount;
    doctorCapacity = other.doctorCapacity;
    serviceCount = other.serviceCount;
    serviceCapacity = other.serviceCapacity;

    doctors = new Doctor * [doctorCapacity] {nullptr};
    for (int i = 0; i < doctorCount && i < doctorCapacity; i++) {
        doctors[i] = other.doctors[i];
    }

    services = new Service * [serviceCapacity] {nullptr};
    for (int i = 0; i < serviceCount && i < serviceCapacity; i++) {
        services[i] = other.services[i];
    }

    return *this;
}

Filial::~Filial() {
    delete[] doctors;
    delete[] services;
}

void Filial::setName(const std::string& newName) { name = newName; }
void Filial::setAddress(const std::string& newAddress) { address = newAddress; }

bool Filial::addDoctor(Doctor* doctor) {
    if (doctorCount >= doctorCapacity) {

        std::cout << "[ERROR] Filial \"" << name << "\" is full. Cannot add doctor.\n";
        return false;
    }    for (int i = 0; i < doctorCount; i++) {
        if (doctors[i]->getId() == doctor->getId()) {
            std::cout << " Doctor is already assigned to this filial.\n";
            return false;
        }
    }

    doctors[doctorCount] = doctor;
    doctorCount++;
    std::cout << " Doctor " << doctor->getFio() << " assigned to filial \"" << name << "\".\n";
    return true;
}

bool Filial::addService(Service* service) {
    if (serviceCount >= serviceCapacity) {
        std::cout << " Filial \"" << name << "\" cannot offer more services.\n";
        return false;
    }
    for (int i = 0; i < serviceCount; i++) {
        if (services[i]->getId() == service->getId()) {
            std::cout << " Service is already available in this filial.\n";
            return false;
        }
    }
    services[serviceCount] = service;
    serviceCount++;
    std::cout << " Service \"" << service->getName() << "\" added to filial.\n";
    return true;
}

bool Filial::isServiceAvailable(int serviceId) const {
    for (int i = 0; i < serviceCount; i++) {
        if (services[i]->getId() == serviceId) return true;
    }
    return false;
}

std::string Filial::getName() const { return name; }
std::string Filial::getAddress() const { return address; }
int Filial::getDoctorCount() const { return doctorCount; }
int Filial::getServiceCount() const { return serviceCount; }

void Filial::printInfo() const {
    std::cout << "\n=== FILIAL: " << name << " ===\n";
    std::cout << "Address: " << address << "\n";

    std::cout << "Doctors (" << doctorCount << "/" << doctorCapacity << "):\n";
    if (doctorCount == 0) std::cout << "  (Empty)\n";
    for (int i = 0; i < doctorCount; i++) {
        std::cout << "  - " << doctors[i]->getFio() << " (" << doctors[i]->getSpecialty() << ")\n";
    }

    std::cout << "Services (" << serviceCount << "/" << serviceCapacity << "):\n";
    if (serviceCount == 0) std::cout << "  (Empty)\n";
    for (int i = 0; i < serviceCount; i++) {
        std::cout << "  - " << services[i]->getName() << " (" << services[i]->getPrice() << " USD)\n";
    }
}