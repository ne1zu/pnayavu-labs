#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <iostream>
#include "doctor.h"
#include "service.h"
#include "patient.h"
#include "clinic_record.h"

#define  MEMBERSIZE  10
class Filial {
private:

    std::string name;
    std::string address;

    std::vector<std::shared_ptr<Doctor>> doctors;
    std::vector<std::shared_ptr<Service>> services;
    std::vector<std::shared_ptr<Patient>> patients;

    int doctorCapacity;
    int serviceCapacity;
    int patientCapacity;

public:
    Filial(std::string_view name = "", std::string_view address = "",
        int doctorCapacity = MEMBERSIZE, int serviceCapacity = MEMBERSIZE, int patientCapacity = MEMBERSIZE);

    std::string getName() const;
    std::string getAddress() const;
    int getDoctorCount() const;
    int getServiceCount() const;
    int getPatientCount() const;

    void setName(std::string_view nameValue);
    void setAddress(std::string_view addressValue);

    Filial& operator+=(std::shared_ptr<Doctor> doctor);
    Filial& operator+=(std::shared_ptr<Service> service);
    Filial& operator+=(std::shared_ptr<Patient> patient);

    Filial& operator-=(std::shared_ptr<Doctor> doctor);
    Filial& operator-=(std::shared_ptr<Service> service);
    Filial& operator-=(std::shared_ptr<Patient> patient);

    
    std::vector<std::shared_ptr<ClinicRecord>> getAllEntities() const;

    friend std::ostream& operator<<(std::ostream& os, const Filial& filial);

private:
    
    bool hasSpaceForDoctor() const;
    bool hasSpaceForService() const;
    bool hasSpaceForPatient() const;
    bool hasAnyDoctor() const;

    int indexOfDoctor(const std::shared_ptr<Doctor>& doctor) const;
    int indexOfService(const std::shared_ptr<Service>& service) const;
    int indexOfPatient(const std::shared_ptr<Patient>& patient) const;

    
    friend bool isServiceAvailable(const Filial& filial, const Service& service);
    friend std::shared_ptr<Doctor> findDoctorBySpecialty(const Filial& filial, std::string_view specialty);
};
