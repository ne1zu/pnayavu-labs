#include "doctor.h"
#include <iostream>

int Doctor::counter = 0;

Doctor::Doctor(std::string_view fio, std::string_view specialty, int experience)
    : fio(fio), specialty(specialty), experience(experience)
{
    id = counter++;
}

int Doctor::getId() const { return id; }
std::string Doctor::getFio() const { return fio; }
std::string Doctor::getSpecialty() const { return specialty; }
int Doctor::getExperience() const { return experience; }

void Doctor::setFio(std::string_view fioValue) { fio = fioValue; }
void Doctor::setSpecialty(std::string_view specialtyValue) { specialty = specialtyValue; }
void Doctor::setExperience(int expValue) { experience = expValue; }

void Doctor::printInfo() const {
    std::cout << "Doctor #" << id << "\n"
        << "  Name: " << fio << "\n"
        << "  Specialty: " << specialty << "\n"
        << "  Experience: " << experience << " years\n";
}