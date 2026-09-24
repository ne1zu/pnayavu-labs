#include "filial.h"

Filial::Filial(std::string_view name, std::string_view address,
    size_t doctorCapacity, size_t serviceCapacity, size_t patientCapacity)
    : name(name), address(address),
      doctorCapacity(doctorCapacity), serviceCapacity(serviceCapacity), patientCapacity(patientCapacity) {
}

std::string Filial::getName() const { return name; }
std::string Filial::getAddress() const { return address; }
int Filial::getDoctorCount() const { return static_cast<int>(doctors.size()); }
int Filial::getServiceCount() const { return static_cast<int>(services.size()); }
int Filial::getPatientCount() const { return static_cast<int>(patients.size()); }

void Filial::setName(std::string_view nameValue) { name = nameValue; }
void Filial::setAddress(std::string_view addressValue) { address = addressValue; }


Filial& Filial::operator+=(std::shared_ptr<Doctor> doctor) {
    if (doctors.size() >= doctorCapacity) {
        std::cout << "[ERROR] Doctor capacity reached for filial \"" << name << "\"!\n";
        return *this;
    }
    doctors.push_back(doctor);
    return *this;
}

Filial& Filial::operator+=(std::shared_ptr<Service> service) {
    if (services.size() >= serviceCapacity) {
        std::cout << "[ERROR] Service capacity reached for filial \"" << name << "\"!\n";
        return *this;
    }
    services.push_back(service);
    return *this;
}

Filial& Filial::operator+=(std::shared_ptr<Patient> patient) {
    if (patients.size() >= patientCapacity) {
        std::cout << "[ERROR] Patient capacity reached for filial \"" << name << "\"!\n";
        return *this;
    }
    patients.push_back(patient);
    return *this;
}


Filial& Filial::operator-=(std::shared_ptr<Doctor> doctor) {
    for (size_t i = 0; i < doctors.size(); ++i) {
        if (*doctors[i] == *doctor) {
            doctors.erase(doctors.begin() + i);
            return *this;
        }
    }
    std::cout << "[ERROR] Doctor not found in filial \"" << name << "\"!\n";
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Service> service) {
    for (size_t i = 0; i < services.size(); ++i) {
        if (*services[i] == *service) {
            services.erase(services.begin() + i);
            return *this;
        }
    }
    std::cout << "[ERROR] Service not found in filial \"" << name << "\"!\n";
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Patient> patient) {
    for (size_t i = 0; i < patients.size(); ++i) {
        if (*patients[i] == *patient) {
            patients.erase(patients.begin() + i);
            return *this;
        }
    }
    std::cout << "[ERROR] Patient not found in filial \"" << name << "\"!\n";
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Filial& filial) {
    os << "Filial: " << filial.name << " (" << filial.address << ")\n";

    os << "  Doctors (" << filial.doctors.size() << "/" << filial.doctorCapacity << "):\n";
    for (const auto& d : filial.doctors) os << "    " << *d << "\n";

    os << "  Services (" << filial.services.size() << "/" << filial.serviceCapacity << "):\n";
    for (const auto& s : filial.services) os << "    " << *s << "\n";

    os << "  Patients (" << filial.patients.size() << "/" << filial.patientCapacity << "):\n";
    for (const auto& p : filial.patients) os << "    " << *p << "\n";

    return os;
}

bool isServiceAvailable(const Filial& filial, const Service& service) {
    for (size_t i = 0; i < filial.services.size(); ++i) {
        if (*filial.services[i] == service) {
            return true;
        }
    }
    return false;
}


std::shared_ptr<Doctor> findDoctorBySpecialty(const Filial& filial, std::string_view specialty) {
    for (size_t i = 0; i < filial.doctors.size(); ++i) {
        if (filial.doctors[i]->getSpecialty() == specialty) {
            return filial.doctors[i];
        }
    }
    return nullptr;
}
