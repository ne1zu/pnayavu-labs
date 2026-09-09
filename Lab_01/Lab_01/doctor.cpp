#include "doctor.h"
#include <iostream>

int Doctor::counter = 0;

Doctor::Doctor(const std::string& fio, const std::string& specialty, int experience)
    : fio(fio), specialty(specialty), experience(experience)
{
    id = counter;
    counter++;
}

int Doctor::getId() const { return id; }
std::string Doctor::getFio() const { return fio; }
std::string Doctor::getSpecialty() const { return specialty; }
int Doctor::getExperience() const { return experience; }

void Doctor::setSpecialty(const std::string& specialty) {
    this->specialty = specialty;
}

void Doctor::setExperience(int experience) {
    this->experience = experience;
}

void Doctor::printInfo() const {
    std::cout << "Врач #" << id << "\n";
    std::cout << "ФИО: " << fio << "\n";
    std::cout << "Специальность: " << specialty << "\n";
    std::cout << "Стаж: " << experience << " лет\n";
}