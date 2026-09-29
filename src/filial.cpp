#include "filial.h"


    void printCapacityError(const std::string& filialName, const std::string& objectType) {
        std::cout << "ERROR " << objectType << " capacity reached for filial \"" << filialName << "\"!\n";
    }

    void printNotFoundError(const std::string& filialName, const std::string& objectType) {
        std::cout << "ERROR " << objectType << " not found in filial \"" << filialName << "\"!\n";
    }

    void printDuplicateError(const std::string& filialName, const std::string& objectType) {
        std::cout << " This " << objectType << " is already assigned to filial \"" << filialName << "\"!\n";
    }

    void printNullObjectError(const std::string& filialName, const std::string& objectType) {
        std::cout << " Cannot add an empty " << objectType << " reference to filial \"" << filialName << "\"!\n";
    }

    void printNoDoctorsError(const std::string& filialName) {
        std::cout << "Cannot add a Patient to filial \"" << filialName << "\" — no doctors are assigned there yet!\n";
    }


Filial::Filial(std::string_view name, std::string_view address,
    int doctorCapacity, int serviceCapacity, int patientCapacity)
    : name(name), address(address),
    doctorCapacity(doctorCapacity), serviceCapacity(serviceCapacity), patientCapacity(patientCapacity) {
}

std::string Filial::getName() const { return name; }
std::string Filial::getAddress() const { return address; }

int Filial::getDoctorCount() const {
    int count = doctors.size();
    return count;
}
int Filial::getServiceCount() const {
    int count = services.size();
    return count;
}
int Filial::getPatientCount() const {
    int count = patients.size();
    return count;
}

void Filial::setName(std::string_view nameValue) { name = nameValue; }
void Filial::setAddress(std::string_view addressValue) { address = addressValue; }

bool Filial::hasSpaceForDoctor() const {
    int count = doctors.size();
    return count < doctorCapacity;
}
bool Filial::hasSpaceForService() const {
    int count = services.size();
    return count < serviceCapacity;
}
bool Filial::hasSpaceForPatient() const {
    int count = patients.size();
    return count < patientCapacity;
}
bool Filial::hasAnyDoctor() const {
    return !doctors.empty();
}

int Filial::indexOfDoctor(const std::shared_ptr<Doctor>& doctor) const {
    int count = doctors.size();
    for (int i = 0; i < count; ++i) {
        if (*doctors[i] == *doctor) return i;
    }
    return -1;
}

int Filial::indexOfService(const std::shared_ptr<Service>& service) const {
    int count = services.size();
    for (int i = 0; i < count; ++i) {
        if (*services[i] == *service) return i;
    }
    return -1;
}

int Filial::indexOfPatient(const std::shared_ptr<Patient>& patient) const {
    int count = patients.size();
    for (int i = 0; i < count; ++i) {
        if (*patients[i] == *patient) return i;
    }
    return -1;
}

Filial& Filial::operator+=(std::shared_ptr<Doctor> doctor) {
    if (!doctor) {
        printNullObjectError(name, "Doctor");
        return *this;
    }
    if (indexOfDoctor(doctor) >= 0) {
        printDuplicateError(name, "Doctor");
        return *this;
    }
    if (!hasSpaceForDoctor()) {
        printCapacityError(name, "Doctor");
        return *this;
    }
    doctors.push_back(doctor);
    return *this;
}

Filial& Filial::operator+=(std::shared_ptr<Service> service) {
    if (!service) {
        printNullObjectError(name, "Service");
        return *this;
    }
    if (indexOfService(service) >= 0) {
        printDuplicateError(name, "Service");
        return *this;
    }
    if (!hasSpaceForService()) {
        printCapacityError(name, "Service");
        return *this;
    }
    services.push_back(service);
    return *this;
}

Filial& Filial::operator+=(std::shared_ptr<Patient> patient) {
    if (!patient) {
        printNullObjectError(name, "Patient");
        return *this;
    }
    if (!hasAnyDoctor()) {
        printNoDoctorsError(name);
        return *this;
    }
    if (indexOfPatient(patient) >= 0) {
        printDuplicateError(name, "Patient");
        return *this;
    }
    if (!hasSpaceForPatient()) {
        printCapacityError(name, "Patient");
        return *this;
    }
    patients.push_back(patient);
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Doctor> doctor) {
    if (!doctor) {
        printNullObjectError(name, "Doctor");
        return *this;
    }
    int idx = indexOfDoctor(doctor);
    if (idx < 0) {
        printNotFoundError(name, "Doctor");
        return *this;
    }
    doctors.erase(doctors.begin() + idx);
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Service> service) {
    if (!service) {
        printNullObjectError(name, "Service");
        return *this;
    }
    int idx = indexOfService(service);
    if (idx < 0) {
        printNotFoundError(name, "Service");
        return *this;
    }
    services.erase(services.begin() + idx);
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Patient> patient) {
    if (!patient) {
        printNullObjectError(name, "Patient");
        return *this;
    }
    int idx = indexOfPatient(patient);
    if (idx < 0) {
        printNotFoundError(name, "Patient");
        return *this;
    }
    patients.erase(patients.begin() + idx);
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
    int count = filial.services.size();
    for (int i = 0; i < count; ++i) {
        if (*filial.services[i] == service) {
            return true;
        }
    }
    return false;
}

std::shared_ptr<Doctor> findDoctorBySpecialty(const Filial& filial, std::string_view specialty) {
    int count = filial.doctors.size();
    for (int i = 0; i < count; ++i) {
        if (filial.doctors[i]->getSpecialty() == specialty) {
            return filial.doctors[i];
        }
    }
    return nullptr;
}