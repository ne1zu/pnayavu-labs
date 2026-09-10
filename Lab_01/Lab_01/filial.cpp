#include "filial.h"
#include <iostream>

// ИСПРАВЛЕНО: Правильная инициализация списком по стандартам C++
Filial::Filial(const std::string& name, const std::string& address, int doctorCapacity, int serviceCapacity)
    : name(name), address(address),
    doctors(new Doctor* [doctorCapacity] {nullptr}), doctorCount(0), doctorCapacity(doctorCapacity),
    services(new Service* [serviceCapacity] {nullptr}), serviceCount(0), serviceCapacity(serviceCapacity)
{
}


Filial::Filial(const Filial& other)
    : name(other.name), address(other.address),
    doctors(new Doctor* [other.doctorCapacity] ), doctorCount(other.doctorCount), doctorCapacity(other.doctorCapacity),
    services(new Service* [other.serviceCapacity] ), serviceCount(other.serviceCount), serviceCapacity(other.serviceCapacity)
{
    for (int i = 0; i < doctorCount && i < doctorCapacity; i++) doctors[i] = other.doctors[i];
    for (int i = 0; i < serviceCount && i < serviceCapacity; i++) services[i] = other.services[i];
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
    for (int i = 0; i < doctorCount && i < doctorCapacity; i++) doctors[i] = other.doctors[i];

    services = new Service * [serviceCapacity] {nullptr};
    for (int i = 0; i < serviceCount && i < serviceCapacity; i++) services[i] = other.services[i];

    return *this;
}

Filial::~Filial() {
    delete[] doctors;
    delete[] services;
}

void Filial::setName(const std::string& nameValue) { name = nameValue; }
void Filial::setAddress(const std::string& addressValue) { address = addressValue; }

bool Filial::addDoctor(Doctor* doctor) {
    if (doctorCount >= doctorCapacity) {
        std::cout << "Error: branch \"" << name << "\" is full, cannot add doctor "
            << doctor->getFio() << " (capacity: " << doctorCapacity << ")\n";
        return false;
    }
    doctors[doctorCount] = doctor;
    doctorCount++;
    std::cout << "Doctor " << doctor->getFio() << " added to branch \"" << name << "\"\n";
    return true;
}

bool Filial::addService(Service* service) {
    if (serviceCount >= serviceCapacity) {
        std::cout << "Error: branch \"" << name << "\" cannot offer more services "
            << "(capacity: " << serviceCapacity << ")\n";
        return false;
    }
    for (int i = 0; i < serviceCount; i++) {
        if (services[i]->getId() == service->getId()) {
            std::cout << "Service \"" << service->getName() << "\" is already available in branch \"" << name << "\"\n";
            return false;
        }
    }
    services[serviceCount] = service;
    serviceCount++;
    std::cout << "Service \"" << service->getName() << "\" added to branch \"" << name << "\"\n";
    return true;
}

bool Filial::isServiceAvailable(int serviceId) const {
    for (int i = 0; i < serviceCount; i++) {
        if (services[i]->getId() == serviceId) {
            return true;
        }
    }
    return false;
}

std::string Filial::getName() const { return name; }
std::string Filial::getAddress() const { return address; }
int Filial::getDoctorCount() const { return doctorCount; }
int Filial::getServiceCount() const { return serviceCount; }

void Filial::printInfo() const {
    std::cout << "===== Branch: " << name << " =====\n";
    std::cout << "Address: " << address << "\n";
    std::cout << "Doctors (" << doctorCount << "/" << doctorCapacity << "):\n";
    if (doctorCount == 0) {
        std::cout << "  (no doctors)\n";
    }
    for (int i = 0; i < doctorCount; i++) {
        std::cout << "  - " << doctors[i]->getFio() << " (" << doctors[i]->getSpecialty() << ")\n";
    }
    std::cout << "Services (" << serviceCount << "/" << serviceCapacity << "):\n";
    if (serviceCount == 0) {
        std::cout << "  (no services)\n";
    }
    for (int i = 0; i < serviceCount; i++) {
        std::cout << "  - " << services[i]->getName() << " (" << services[i]->getPrice() << " USD)\n";
    }
}