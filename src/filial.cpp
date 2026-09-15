#include "filial.h"
#include <iostream>

Filial::Filial(std::string_view name, std::string_view address, int docCap, int servCap)
    : name(name), address(address), doctorCapacity(docCap), serviceCapacity(servCap)
{
}

void Filial::setName(std::string_view newName) { name = newName; }
void Filial::setAddress(std::string_view newAddress) { address = newAddress; }

bool Filial::addDoctor(std::shared_ptr<Doctor> doctor) {
    if (doctors.size() >= doctorCapacity) {
        std::cout << "[ERROR] Filial \"" << name << "\" is full. Cannot add doctor.\n";
        return false;
    }
    for (const auto& doc : doctors) {
        if (doc->getId() == doctor->getId()) {
            std::cout << " Doctor is already assigned to this filial.\n";
            return false;
        }
    }
    doctors.push_back(doctor); 
    std::cout << " Doctor " << doctor->getFio() << " assigned to filial \"" << name << "\".\n";
    return true;
}

bool Filial::addService(std::shared_ptr<Service> service) {
    if (services.size() >= serviceCapacity) {
        std::cout << " Filial \"" << name << "\" cannot offer more services.\n";
        return false;
    }
    for (const auto& srv : services) {
        if (srv->getId() == service->getId()) {
            std::cout << " Service is already available in this filial.\n";
            return false;
        }
    }
    services.push_back(service);
    std::cout << " Service \"" << service->getName() << "\" added to filial.\n";
    return true;
}

bool Filial::isServiceAvailable(int serviceId) const {
    for (const auto& srv : services) {
        if (srv->getId() == serviceId) return true;
    }
    return false;
}

std::string Filial::getName() const { return name; }
std::string Filial::getAddress() const { return address; }
int Filial::getDoctorCount() const { return doctors.size(); } 
int Filial::getServiceCount() const { return services.size(); }

void Filial::printInfo() const {
    std::cout << "\n=== FILIAL: " << name << " ===\n"
        << "Address: " << address << "\n";

    std::cout << "Doctors (" << doctors.size() << "/" << doctorCapacity << "):\n";
    if (doctors.empty()) std::cout << "  (Empty)\n";
    for (const auto& doc : doctors) {
        std::cout << "  - " << doc->getFio() << " (" << doc->getSpecialty() << ")\n";
    }

    std::cout << "Services (" << services.size() << "/" << serviceCapacity << "):\n";
    if (services.empty()) std::cout << "  (Empty)\n";
    for (const auto& srv : services) {
        std::cout << "  - " << srv->getName() << " (" << srv->getPrice() << " USD)\n";
    }
}