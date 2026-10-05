#pragma once
#include <vector>
#include <memory>
#include "doctor.h"
#include "patient.h"
#include "administrator.h"
#include "service.h"
#include "person.h"

#define MEMBERSIZE 10

class Filial {
private:
    std::string name;
    std::string address;

    std::vector<std::shared_ptr<Doctor>> doctors;
    std::vector<std::shared_ptr<Patient>> patients;
    std::vector<std::shared_ptr<Administrator>> admins;
    std::vector<std::shared_ptr<Service>> services;

    int capacity;

public:
    Filial(std::string_view name = "", std::string_view address = "", int capacity = MEMBERSIZE);

    std::string getName() const;
    std::string getAddress() const;

    Filial& operator+=(std::shared_ptr<Doctor> doctor);
    Filial& operator+=(std::shared_ptr<Patient> patient);
    Filial& operator+=(std::shared_ptr<Administrator> admin);
    Filial& operator+=(std::shared_ptr<Service> service);

    Filial& operator-=(std::shared_ptr<Doctor> doctor);
    Filial& operator-=(std::shared_ptr<Patient> patient);
    Filial& operator-=(std::shared_ptr<Administrator> admin);
    Filial& operator-=(std::shared_ptr<Service> service);

    // ѕќЋ»ћќ–‘Ќјя  ќЋЋ≈ ÷»я (Ћаба 4) - собирает только людей! ”слуги сюда не вход€т.
    std::vector<std::shared_ptr<Person>> getAllPeople() const;

    friend std::ostream& operator<<(std::ostream& os, const Filial& filial);
    friend bool isServiceAvailable(const Filial& filial, const Service& service);
    friend std::shared_ptr<Administrator> findAdminByPosition(const Filial& filial, std::string_view position);
};