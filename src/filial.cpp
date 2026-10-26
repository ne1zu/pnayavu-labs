#include "filial.h"
#include <sstream>
#include <iomanip>
#include "exceptions/exceptions.h"

namespace {
    std::string branchLabel(const std::string& branchName) {
        return "Branch '" + branchName + "'";
    }

    std::string formatClock(int totalMinutes) {
        std::ostringstream out;
        out << std::setfill('0') << std::setw(2) << totalMinutes / 60 << ":" << std::setw(2) << totalMinutes % 60;
        return out.str();
    }

    std::string formatRawDateTime(const Appointment& appointment) {
        std::ostringstream out;
        out << std::setfill('0')
            << std::setw(2) << appointment.getDay() << "." << std::setw(2) << appointment.getMonth() << "."
            << appointment.getYear() << " "
            << std::setw(2) << appointment.getHour() << ":" << std::setw(2) << appointment.getMinute();
        return out.str();
    }

    int checkedCapacity(int value, const std::string& kind) {
        if (value < 0) {
            throw InvalidDataException(kind + " capacity", "must not be negative, got " + std::to_string(value));
        }
        return value;
    }

    void printCancelledInfo(const std::string& filialName, int count, const std::string& objectType) {
        std::cout << "[INFO] Branch '" << filialName << "': " << count
            << " appointment(s) cancelled because the " << objectType << " was removed.\n";
    }
}

Filial::Filial(std::string_view name, std::string_view address,
    int doctorCapacity, int serviceCapacity, int patientCapacity, int administratorCapacity,
    int appointmentCapacity)
    : name(name), address(address),
    doctorCapacity(checkedCapacity(doctorCapacity, "doctor")),
    serviceCapacity(checkedCapacity(serviceCapacity, "service")),
    patientCapacity(checkedCapacity(patientCapacity, "patient")),
    administratorCapacity(checkedCapacity(administratorCapacity, "administrator")),
    appointmentCapacity(checkedCapacity(appointmentCapacity, "appointment")) {
}

std::string Filial::getName() const { return name; }
std::string Filial::getAddress() const { return address; }

int Filial::getDoctorCount() const { return doctors.getCount(); }
int Filial::getServiceCount() const { return services.getCount(); }
int Filial::getPatientCount() const { return patients.getCount(); }
int Filial::getAdministratorCount() const { return administrators.getCount(); }
int Filial::getAppointmentCount() const { return appointments.getCount(); }

int Filial::getDoctorCapacity() const { return doctorCapacity; }
int Filial::getServiceCapacity() const { return serviceCapacity; }
int Filial::getPatientCapacity() const { return patientCapacity; }
int Filial::getAdministratorCapacity() const { return administratorCapacity; }
int Filial::getAppointmentCapacity() const { return appointmentCapacity; }

Collection<Doctor> Filial::getDoctors() const { return doctors; }
Collection<Service> Filial::getServices() const { return services; }
Collection<Patient> Filial::getPatients() const { return patients; }
Collection<Administrator> Filial::getAdministrators() const { return administrators; }
Collection<Appointment> Filial::getAppointments() const { return appointments; }

bool Filial::contains(const std::shared_ptr<Doctor>& doctor) const { return doctor && indexOfDoctor(doctor) >= 0; }
bool Filial::contains(const std::shared_ptr<Service>& service) const { return service && indexOfService(service) >= 0; }
bool Filial::contains(const std::shared_ptr<Patient>& patient) const { return patient && indexOfPatient(patient) >= 0; }
bool Filial::contains(const std::shared_ptr<Administrator>& administrator) const { return administrator && indexOfAdministrator(administrator) >= 0; }

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
    for (const auto& existing : appointments) {
        if (*existing->getDoctor() == *candidate.getDoctor() && existing->overlaps(candidate)) {
            return true;
        }
    }
    return false;
}

bool Filial::isPatientBusy(const Appointment& candidate) const {
    for (const auto& existing : appointments) {
        if (*existing->getPatient() == *candidate.getPatient() && existing->overlaps(candidate)) {
            return true;
        }
    }
    return false;
}

int Filial::cancelAppointmentsOfDoctor(const Doctor& doctor) {
    int cancelled = 0;
    for (int i = appointments.getCount() - 1; i >= 0; i--) {
        if (*appointments.get(i)->getDoctor() == doctor) {
            appointments.removeAt(i);
            cancelled++;
        }
    }
    return cancelled;
}

int Filial::cancelAppointmentsOfPatient(const Patient& patient) {
    int cancelled = 0;
    for (int i = appointments.getCount() - 1; i >= 0; i--) {
        if (*appointments.get(i)->getPatient() == patient) {
            appointments.removeAt(i);
            cancelled++;
        }
    }
    return cancelled;
}

int Filial::cancelAppointmentsOfService(const Service& service) {
    int cancelled = 0;
    for (int i = appointments.getCount() - 1; i >= 0; i--) {
        if (*appointments.get(i)->getService() == service) {
            appointments.removeAt(i);
            cancelled++;
        }
    }
    return cancelled;
}

Filial& Filial::operator+=(std::shared_ptr<Doctor> doctor) {
    if (!doctor) throw NullObjectException(branchLabel(name), "Doctor");
    if (indexOfDoctor(doctor) >= 0) throw DuplicateObjectException(branchLabel(name), doctor->getEntityType());
    if (!hasSpaceForDoctor()) throw CapacityExceededException(branchLabel(name), doctor->getEntityType(), doctorCapacity);
    doctors.add(doctor);
    return *this;
}

Filial& Filial::operator+=(std::shared_ptr<Service> service) {
    if (!service) throw NullObjectException(branchLabel(name), "Service");
    if (indexOfService(service) >= 0) throw DuplicateObjectException(branchLabel(name), service->getEntityType());
    if (!hasSpaceForService()) throw CapacityExceededException(branchLabel(name), service->getEntityType(), serviceCapacity);
    services.add(service);
    return *this;
}

Filial& Filial::operator+=(std::shared_ptr<Patient> patient) {
    if (!patient) throw NullObjectException(branchLabel(name), "Patient");
    if (!hasAnyDoctor()) {
        throw InvalidOperationException(branchLabel(name), "add " + patient->getEntityType(),
            "add a doctor to the branch first");
    }
    storePatient(patient);
    return *this;
}

void Filial::restorePatient(std::shared_ptr<Patient> patient) {
    if (!patient) throw NullObjectException(branchLabel(name), "Patient");
    storePatient(patient);
}

void Filial::storePatient(const std::shared_ptr<Patient>& patient) {
    if (indexOfPatient(patient) >= 0) throw DuplicateObjectException(branchLabel(name), patient->getEntityType());
    if (!hasSpaceForPatient()) throw CapacityExceededException(branchLabel(name), patient->getEntityType(), patientCapacity);
    patients.add(patient);
}

Filial& Filial::operator+=(std::shared_ptr<Administrator> administrator) {
    if (!administrator) throw NullObjectException(branchLabel(name), "Administrator");
    if (indexOfAdministrator(administrator) >= 0) throw DuplicateObjectException(branchLabel(name), administrator->getEntityType());
    if (!hasSpaceForAdministrator()) throw CapacityExceededException(branchLabel(name), administrator->getEntityType(), administratorCapacity);
    administrators.add(administrator);
    return *this;
}

Filial& Filial::operator+=(std::shared_ptr<Appointment> appointment) {
    if (!appointment) throw NullObjectException(branchLabel(name), "Appointment");
    if (!appointment->isComplete()) {
        throw LinkViolationException(branchLabel(name), "appointment must be linked to a patient, a doctor and a service");
    }
    if (appointment->getFilialName() != name) {
        throw LinkViolationException(branchLabel(name),
            "appointment belongs to branch '" + appointment->getFilialName() + "', not to this branch");
    }
    if (indexOfPatient(appointment->getPatient()) < 0) {
        throw LinkViolationException(branchLabel(name),
            "Patient '" + appointment->getPatient()->getName() + "' is not registered at this branch");
    }
    if (indexOfDoctor(appointment->getDoctor()) < 0) {
        throw LinkViolationException(branchLabel(name),
            "Doctor '" + appointment->getDoctor()->getName() + "' is not registered at this branch");
    }
    if (indexOfService(appointment->getService()) < 0) {
        throw LinkViolationException(branchLabel(name),
            "Service '" + appointment->getService()->getName() + "' is not registered at this branch");
    }
    if (!appointment->hasValidDateTime()) {
        throw InvalidDateTimeException("'" + formatRawDateTime(*appointment) + "' is not a valid date and time");
    }
    if (!appointment->fitsWorkingHours()) {
        throw WorkingHoursException(appointment->getDateTimeString(),
            formatClock(Appointment::WORK_START) + "-" + formatClock(Appointment::WORK_END));
    }
    if (indexOfAppointment(appointment) >= 0) throw DuplicateObjectException(branchLabel(name), appointment->getEntityType());
    if (isDoctorBusy(*appointment)) {
        throw ScheduleConflictException(appointment->getDoctor()->getEntityType(),
            appointment->getDoctor()->getName(), appointment->getDateTimeString());
    }
    if (isPatientBusy(*appointment)) {
        throw ScheduleConflictException(appointment->getPatient()->getEntityType(),
            appointment->getPatient()->getName(), appointment->getDateTimeString());
    }
    if (!hasSpaceForAppointment()) throw CapacityExceededException(branchLabel(name), appointment->getEntityType(), appointmentCapacity);
    appointments.add(appointment);
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Doctor> doctor) {
    if (!doctor) throw NullObjectException(branchLabel(name), "Doctor");
    int index = indexOfDoctor(doctor);
    if (index < 0) throw ObjectNotFoundException(branchLabel(name), doctor->getEntityType());
    doctors.removeAt(index);
    int cancelled = cancelAppointmentsOfDoctor(*doctor);
    if (cancelled > 0) printCancelledInfo(name, cancelled, doctor->getEntityType());
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Service> service) {
    if (!service) throw NullObjectException(branchLabel(name), "Service");
    int index = indexOfService(service);
    if (index < 0) throw ObjectNotFoundException(branchLabel(name), service->getEntityType());
    services.removeAt(index);
    int cancelled = cancelAppointmentsOfService(*service);
    if (cancelled > 0) printCancelledInfo(name, cancelled, service->getEntityType());
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Patient> patient) {
    if (!patient) throw NullObjectException(branchLabel(name), "Patient");
    int index = indexOfPatient(patient);
    if (index < 0) throw ObjectNotFoundException(branchLabel(name), patient->getEntityType());
    patients.removeAt(index);
    int cancelled = cancelAppointmentsOfPatient(*patient);
    if (cancelled > 0) printCancelledInfo(name, cancelled, patient->getEntityType());
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Administrator> administrator) {
    if (!administrator) throw NullObjectException(branchLabel(name), "Administrator");
    int index = indexOfAdministrator(administrator);
    if (index < 0) throw ObjectNotFoundException(branchLabel(name), administrator->getEntityType());
    administrators.removeAt(index);
    return *this;
}

Filial& Filial::operator-=(std::shared_ptr<Appointment> appointment) {
    if (!appointment) throw NullObjectException(branchLabel(name), "Appointment");
    int index = indexOfAppointment(appointment);
    if (index < 0) throw ObjectNotFoundException(branchLabel(name), appointment->getEntityType());
    appointments.removeAt(index);
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
