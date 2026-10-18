#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <iostream>
#include "doctor.h"
#include "service.h"
#include "patient.h"
#include "administrator.h"
#include "person.h"
#include "appointment.h"
#include "collection.h"

const int DEFAULT_CAPACITY = 10;

class Filial {
private:
    std::string name;
    std::string address;

    Collection<Doctor> doctors;
    Collection<Service> services;
    Collection<Patient> patients;
    Collection<Administrator> administrators;
    Collection<Appointment> appointments;

    int doctorCapacity;
    int serviceCapacity;
    int patientCapacity;
    int administratorCapacity;
    int appointmentCapacity;

public:
    Filial(std::string_view name = "", std::string_view address = "",
        int doctorCapacity = DEFAULT_CAPACITY, int serviceCapacity = DEFAULT_CAPACITY, int patientCapacity = DEFAULT_CAPACITY,
        int administratorCapacity = DEFAULT_CAPACITY, int appointmentCapacity = DEFAULT_CAPACITY);

    std::string getName() const;
    std::string getAddress() const;
    int getDoctorCount() const;
    int getServiceCount() const;
    int getPatientCount() const;
    int getAdministratorCount() const;
    int getAppointmentCount() const;

    void setName(std::string_view nameValue);
    void setAddress(std::string_view addressValue);

    Filial& operator+=(std::shared_ptr<Doctor> doctor);
    Filial& operator+=(std::shared_ptr<Service> service);
    Filial& operator+=(std::shared_ptr<Patient> patient);
    Filial& operator+=(std::shared_ptr<Administrator> administrator);
    Filial& operator+=(std::shared_ptr<Appointment> appointment);

    Filial& operator-=(std::shared_ptr<Doctor> doctor);
    Filial& operator-=(std::shared_ptr<Service> service);
    Filial& operator-=(std::shared_ptr<Patient> patient);
    Filial& operator-=(std::shared_ptr<Administrator> administrator);
    Filial& operator-=(std::shared_ptr<Appointment> appointment);

    Collection<Doctor> getDoctors() const;
    Collection<Service> getServices() const;
    Collection<Patient> getPatients() const;
    Collection<Administrator> getAdministrators() const;
    Collection<Appointment> getAppointments() const;

    bool contains(const std::shared_ptr<Doctor>& doctor) const;
    bool contains(const std::shared_ptr<Service>& service) const;
    bool contains(const std::shared_ptr<Patient>& patient) const;
    bool contains(const std::shared_ptr<Administrator>& administrator) const;

    std::vector<std::shared_ptr<Person>> getAllEntities() const;

    friend std::ostream& operator<<(std::ostream& os, const Filial& filial);

private:
    bool hasSpaceForDoctor() const;
    bool hasSpaceForService() const;
    bool hasSpaceForPatient() const;
    bool hasSpaceForAdministrator() const;
    bool hasSpaceForAppointment() const;
    bool hasAnyDoctor() const;

    int indexOfDoctor(const std::shared_ptr<Doctor>& doctor) const;
    int indexOfService(const std::shared_ptr<Service>& service) const;
    int indexOfPatient(const std::shared_ptr<Patient>& patient) const;
    int indexOfAdministrator(const std::shared_ptr<Administrator>& administrator) const;
    int indexOfAppointment(const std::shared_ptr<Appointment>& appointment) const;

    bool isDoctorBusy(const Appointment& candidate) const;
    bool isPatientBusy(const Appointment& candidate) const;
    int cancelAppointmentsOfDoctor(const Doctor& doctor);
    int cancelAppointmentsOfPatient(const Patient& patient);
    int cancelAppointmentsOfService(const Service& service);

    friend bool isServiceAvailable(const Filial& filial, const Service& service);
    friend std::shared_ptr<Doctor> findDoctorBySpecialty(const Filial& filial, std::string_view specialty);
    friend std::shared_ptr<Administrator> findAdministratorByPosition(const Filial& filial, std::string_view position);
};
