#include "filial.h"

namespace {
    void printCapacityError(const std::string& filialName, const std::string& objectType) {
        std::cout << "[ERROR] Capacity reached!\n";
    }
    void printNotFoundError(const std::string& filialName, const std::string& objectType) {
        std::cout << "[ERROR] Not found!\n";
    }
    void printDuplicateError(const std::string& filialName, const std::string& objectType) {
        std::cout << "[ERROR] Duplicate!\n";
    }
    void printNullObjectError(const std::string& filialName, const std::string& objectType) {
        std::cout << "[ERROR] Null object!\n";
    }
    void printNotRegisteredError(const std::string& filialName, const std::string& objectType) {
        std::cout << "[ERROR] Not registered at this branch!\n";
    }
    void printIncompleteAppointmentError() {
        std::cout << "[ERROR] Incomplete appointment!\n";
    }
    void printForeignAppointmentError(const std::string& filialName, const std::string& otherName) {
        std::cout << "[ERROR] Belongs to another branch!\n";
    }
    void printBadDateTimeError() {
        std::cout << "[ERROR] Invalid date/time!\n";
    }
    void printOutsideWorkingHoursError() {
        std::cout << "[ERROR] Outside working hours!\n";
    }
    void printBusyError(const std::string& who) {
        std::cout << "[ERROR] Already has an appointment!\n";
    }
    void printCancelledInfo(const std::string& filialName, int count, const std::string& reason) {
        std::cout << "[INFO] Appointments cancelled.\n";
    }
    void printNoDoctorsError(const std::string& filialName) {
        std::cout << "[ERROR] No doctors assigned!\n";
    }
}

Filial::Filial(std::string_view name, std::string_view address,
    int doctorCapacity, int serviceCapacity, int patientCapacity, int administratorCapacity,
    int appointmentCapacity)
    : name(name), address(address),
    doctorCapacity(doctorCapacity), serviceCapacity(serviceCapacity), patientCapacity(patientCapacity),
    administratorCapacity(administratorCapacity), appointmentCapacity(appointmentCapacity) {
}

std::string Filial::getName() const { return name; }
std::string Filial::getAddress() const { return address; }

int Filial::getDoctorCount() const { return doctors.getCount(); }
int Filial::getServiceCount() const { return services.getCount(); }
int Filial::getPatientCount() const { return patients.getCount(); }
int Filial::getAdministratorCount() const { return administrators.getCount(); }
int Filial::getAppointmentCount() const { return appointments.getCount(); }

Collection<Doctor> Filial::getDoctors() const { return doctors; }
Collection<Service> Filial::getServices() const { return services; }
Collection<Patient> Filial::getPatients() const { return patients; }
Collection<Administrator> Filial::getAdministrators() const { return administrators; }
Collection<Appointment> Filial::getAppointments() const { return appointments; }

void Filial::setName(std::string_view nameValue) {
    name = nameValue;
    for (const auto& appointment : appointments) appointment->setFilialName(nameValue);
}
void Filial::setAddress(std::string_view addressValue) { address = addressValue; }

bool Filial::hasSpaceForDoctor() const { return doctors.getCount() < doctorCapacity; }
bool Filial::hasSpaceForService() const { return services.getCount() < serviceCapacity; }
bool Filial::hasSpaceForPatient() const { return patients.getCount() < patientCapacity; }
bool Filial::hasSpaceForAdministrator() const { return administrators.getCount() < administratorCapacity; }
bool Filial::hasSpaceForAppointment() const { return appointments.getCount() < appointmentCapacity; }
bool Filial::hasAnyDoctor() const { return !doctors.isEmpty(); }

int Filial::indexOfDoctor(const std::shared_ptr<Doctor>& doctor) const { return doctors.indexOf(*doctor); }
int Filial::indexOfService(const std::shared_ptr<Service>& service) const { return services.indexOf(*service); }
int Filial::indexOfPatient(const std::shared_ptr<Patient>& patient) const { return patients.indexOf(*patient); }
int Filial::indexOfAdministrator(const std::shared_ptr<Administrator>& administrator) const { return administrators.indexOf(*administrator); }
int Filial::indexOfAppointment(const std::shared_ptr<Appointment>& appointment) const { return appointments.indexOf(*appointment); }

bool Filial::isDoctorBusy(const Appointment& candidate) const {
    for (const auto& appt : appointments) {
        if (*appt->getDoctor() == *candidate.getDoctor() && appt->overlaps(candidate)) {
            return true;
        }
    }
    return false;
}

bool Filial::isPatientBusy(const Appointment& candidate) const {
    for (const auto& appt : appointments) {
        if (*appt->getPatient() == *candidate.getPatient() && appt->overlaps(candidate)) {
            return true;
        }
    }
    return false;
}

int Filial::cancelAppointmentsOfDoctor(const Doctor& doctor) {
    int count = 0;
    for (const auto& a : appointments) {
        if (*a->getDoctor() == doctor) count++;
    }
    for (int i = appointments.getCount() - 1; i >= 0; i--) {
        if (*appointments.get(i)->getDoctor() == doctor) appointments.removeAt(i);
    }
    return count;
}

int Filial::cancelAppointmentsOfPatient(const Patient& patient) {
    int count = 0;
    for (const auto& a : appointments) {
        if (*a->getPatient() == patient) count++;
    }
    for (int i = appointments.getCount() - 1; i >= 0; i--) {
        if (*appointments.get(i)->getPatient() == patient) appointments.removeAt(i);
    }
    return count;
}

int Filial::cancelAppointmentsOfService(const Service& service) {
    int count = 0;
    for (const auto& a : appointments) {
        if (*a->getService() == service) count++;
    }
    for (int i = appointments.getCount() - 1; i >= 0; i--) {
        if (*appointments.get(i)->getService() == service) appointments.removeAt(i);
    }
    return count;
}

Filial& Filial::operator+=(std::shared_ptr<Doctor> doctor) {
    if (!doctor) { printNullObjectError(name, "Doctor"); return *this; }
    if (indexOfDoctor(doctor) >= 0) { printDuplicateError(name, doctor->getEntityType()); return *this; }
    if (!hasSpaceForDoctor()) { printCapacityError(name, doctor->getEntityType()); return *this; }
    doctors.add(doctor);
    return *this;
}

Filial& Filial::operator+=(std::shared_ptr<Service> service) {
    if (!service) { printNullObjectError(name, "Service"); return *this; }
    if (indexOfService(service) >= 0) { printDuplicateError(name, service->getEntityType()); return *this; }
    if (!hasSpaceForService()) { printCapacityError(name, service->getEntityType()); return *this; }
    services.add(service);
    return *this;
}

Filial& Filial::operator+=(std::shared_ptr<Patient> patient) {
    if (!patient) { printNullObjectError(name, "Patient"); return *this; }
    if (!hasAnyDoctor()) { printNoDoctorsError(name); return *this; }
    if (indexOfPatient(patient) >= 0) { printDuplicateError(name, patient->getEntityType()); return *this; }
    if (!hasSpaceForPatient()) { printCapacityError(name, patient->getEntityType()); return *this; }
    patients.add(patient);
    return *this;
}

Filial& Filial::operator+=(std::shared_ptr<Administrator> administrator) {
    if (!administrator) { printNullObjectError(name, "Administrator"); return *this; }
    if (indexOfAdministrator(administrator) >= 0) { printDuplicateError(name, administrator->getEntityType()); return *this; }
    if (!hasSpaceForAdministrator()) { printCapacityError(name, administrator->getEntityType()); return *this; }
    administrators.add(administrator);
    return *this;
}

Filial& Filial::operator+=(std::shared_ptr<Appointment> appointment) {
    if (!appointment) { printNullObjectError(name, "Appointment"); return *this; }
    if (!appointment->isComplete()) { printIncompleteAppointmentError(); return *this; }
    if (appointment->getFilialName() != name) { printForeignAppointmentError(name, appointment->getFilialName()); return *this; }
    if (indexOfPatient(appointment->getPatient()) < 0) { printNotRegisteredError(name, appointment->getPatient()->getEntityType()); return *this; }
    if (indexOfDoctor(appointment->getDoctor()) < 0) { printNotRegisteredError(name, appointment->getDoctor()->getEntityType()); return *this; }
    if (indexOfService(appointment->getService()) < 0) { printNotRegisteredError(name, appointment->getService()->getEntityType()); return *this; }
    if (!appointment->hasValidDateTime()) { printBadDateTimeError(); return *this; }
    if (!appointment->fitsWorkingHours()) { printOutsideWorkingHoursError(); return *this; }
    if (indexOfAppointment(appointment) >= 0) { printDuplicateError(name, appointment->getEntityType()); return *this; }
    if (isDoctorBusy(*appointment)) { printBusyError(appointment->getDoctor()->getEntityType()); return *this; }
    if (isPatientBusy(*appointment)) { printBusyError(appointment->getPatient()->getEntityType()); return *this; }
    if (!hasSpaceForAppointment()) { printCapacityError(name, appointment->getEntityType()); return *this; }
    appointments.add(appointment);
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Doctor> doctor) {
    if (!doctor) { printNullObjectError(name, "Doctor"); return *this; }
    int idx = indexOfDoctor(doctor);
    if (idx < 0) { printNotFoundError(name, doctor->getEntityType()); return *this; }
    doctors.removeAt(idx);
    int cancelled = cancelAppointmentsOfDoctor(*doctor);
    if (cancelled > 0) printCancelledInfo(name, cancelled, doctor->getEntityType());
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Service> service) {
    if (!service) { printNullObjectError(name, "Service"); return *this; }
    int idx = indexOfService(service);
    if (idx < 0) { printNotFoundError(name, service->getEntityType()); return *this; }
    services.removeAt(idx);
    int cancelled = cancelAppointmentsOfService(*service);
    if (cancelled > 0) printCancelledInfo(name, cancelled, service->getEntityType());
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Patient> patient) {
    if (!patient) { printNullObjectError(name, "Patient"); return *this; }
    int idx = indexOfPatient(patient);
    if (idx < 0) { printNotFoundError(name, patient->getEntityType()); return *this; }
    patients.removeAt(idx);
    int cancelled = cancelAppointmentsOfPatient(*patient);
    if (cancelled > 0) printCancelledInfo(name, cancelled, patient->getEntityType());
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Administrator> administrator) {
    if (!administrator) { printNullObjectError(name, "Administrator"); return *this; }
    int idx = indexOfAdministrator(administrator);
    if (idx < 0) { printNotFoundError(name, administrator->getEntityType()); return *this; }
    administrators.removeAt(idx);
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Appointment> appointment) {
    if (!appointment) { printNullObjectError(name, "Appointment"); return *this; }
    int idx = indexOfAppointment(appointment);
    if (idx < 0) { printNotFoundError(name, appointment->getEntityType()); return *this; }
    appointments.removeAt(idx);
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Filial& filial) {
    os << "Filial: " << filial.name << " (" << filial.address << ")\n";
    os << "  Doctors (" << filial.doctors.getCount() << "/" << filial.doctorCapacity << "):\n";
    for (const auto& d : filial.doctors) os << "    " << *d << " [" << d->classify() << "]\n";
    os << "  Services (" << filial.services.getCount() << "/" << filial.serviceCapacity << "):\n";
    for (const auto& s : filial.services) os << "    " << *s << " [" << s->classify() << "]\n";
    os << "  Patients (" << filial.patients.getCount() << "/" << filial.patientCapacity << "):\n";
    for (const auto& p : filial.patients) os << "    " << *p << " [" << p->classify() << "]\n";
    os << "  Administrators (" << filial.administrators.getCount() << "/" << filial.administratorCapacity << "):\n";
    for (const auto& a : filial.administrators) os << "    " << *a << " [" << a->classify() << "]\n";
    os << "  Appointments (" << filial.appointments.getCount() << "/" << filial.appointmentCapacity << "):\n";
    for (const auto& ap : filial.appointments) os << "    " << *ap << " [" << ap->classify() << "]\n";
    return os;
}

std::vector<std::shared_ptr<Person>> Filial::getAllEntities() const {
    std::vector<std::shared_ptr<Person>> all;
    for (const auto& d : doctors) all.push_back(d);
    for (const auto& p : patients) all.push_back(p);
    for (const auto& a : administrators) all.push_back(a);
    return all;
}

bool isServiceAvailable(const Filial& filial, const Service& service) {
    return filial.services.contains(service);
}

std::shared_ptr<Doctor> findDoctorBySpecialty(const Filial& filial, std::string_view specialty) {
    for (const auto& doctor : filial.doctors) {
        if (doctor->getSpecialty() == specialty) return doctor;
    }
    return nullptr;
}

std::shared_ptr<Administrator> findAdministratorByPosition(const Filial& filial, std::string_view position) {
    for (const auto& admin : filial.administrators) {
        if (admin->getPosition() == position) return admin;
    }
    return nullptr;
}