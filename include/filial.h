#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <iostream>
#include "doctor.h"
#include "service.h"
#include "patient.h"

class Filial {
private:
    std::string name;
    std::string address;

    std::vector<std::shared_ptr<Doctor>> doctors;
    std::vector<std::shared_ptr<Service>> services;
    std::vector<std::shared_ptr<Patient>> patients;

    size_t doctorCapacity;
    size_t serviceCapacity;
    size_t patientCapacity;

public:
    Filial(std::string_view name = "", std::string_view address = "",
        size_t doctorCapacity = 10, size_t serviceCapacity = 10, size_t patientCapacity = 10);

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

    friend std::ostream& operator<<(std::ostream& os, const Filial& filial);

   
    friend bool isServiceAvailable(const Filial& filial, const Service& service);
    friend std::shared_ptr<Doctor> findDoctorBySpecialty(const Filial& filial, std::string_view specialty);
};
