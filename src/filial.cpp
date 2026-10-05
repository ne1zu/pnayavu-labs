#include "filial.h"
#include <iostream>

Filial::Filial(std::string_view name, std::string_view address, int capacity)
    : name(name), address(address), capacity(capacity) {
}

std::string Filial::getName() const { return name; }
std::string Filial::getAddress() const { return address; }

Filial& Filial::operator+=(std::shared_ptr<Doctor> doctor) {
    if (doctors.size() < capacity) doctors.push_back(doctor);
    return *this;
}
Filial& Filial::operator+=(std::shared_ptr<Patient> patient) {
    if (patients.size() < capacity) patients.push_back(patient);
    return *this;
}
Filial& Filial::operator+=(std::shared_ptr<Administrator> admin) {
    if (admins.size() < capacity) admins.push_back(admin);
    return *this;
}
Filial& Filial::operator+=(std::shared_ptr<Service> service) {
    if (services.size() < capacity) services.push_back(service);
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Doctor> doctor) {
    for (auto it = doctors.begin(); it != doctors.end(); ++it) {
        if (*it == doctor) { doctors.erase(it); break; }
    }
    return *this;
}
Filial& Filial::operator-=(std::shared_ptr<Patient> patient) {
    for (auto it = patients.begin(); it != patients.end(); ++it) {
        if (*it == patient) { patients.erase(it); break; }
    }
    return *this;
}
Filial& Filial::operator-=(std::shared_ptr<Administrator> admin) {
    for (auto it = admins.begin(); it != admins.end(); ++it) {
        if (*it == admin) { admins.erase(it); break; }
    }
    return *this;
}
Filial& Filial::operator-=(std::shared_ptr<Service> service) {
    for (auto it = services.begin(); it != services.end(); ++it) {
        if (*it == service) { services.erase(it); break; }
    }
    return *this;
}

std::vector<std::shared_ptr<Person>> Filial::getAllPeople() const {
    std::vector<std::shared_ptr<Person>> all;
    for (const auto& d : doctors) all.push_back(d);
    for (const auto& p : patients) all.push_back(p);
    for (const auto& a : admins) all.push_back(a);
    return all;
}

std::ostream& operator<<(std::ostream& os, const Filial& filial) {
    os << "Filial: " << filial.name << " (" << filial.address << ")\n";
    os << "  Doctors: " << filial.doctors.size() << " | Patients: " << filial.patients.size()
        << " | Admins: " << filial.admins.size() << " | Services: " << filial.services.size() << "\n";
    return os;
}

bool isServiceAvailable(const Filial& filial, const Service& service) {
    for (const auto& s : filial.services) {
        if (*s == service) return true;
    }
    return false;
}

std::shared_ptr<Administrator> findAdminByPosition(const Filial& filial, std::string_view position) {
    for (const auto& a : filial.admins) {
        if (a->getPosition() == position) return a;
    }
    return nullptr;
}