#include "doctor.h"

int Doctor::counter = 0;

Doctor::Doctor(std::string_view fio, std::string_view specialty, int experience)
    : fio(fio), specialty(specialty), experience(experience) {
    id = counter++;
}

int Doctor::getId() const { return id; }
std::string Doctor::getFio() const { return fio; }
std::string Doctor::getSpecialty() const { return specialty; }
int Doctor::getExperience() const { return experience; }

void Doctor::setFio(std::string_view fioValue) { fio = fioValue; }
void Doctor::setSpecialty(std::string_view specialtyValue) { specialty = specialtyValue; }
void Doctor::setExperience(int expValue) {
    if (expValue < 0) {
        std::cout << "[ERROR] Experience cannot be negative!\n";
        return;
    }
    experience = expValue;
}

bool Doctor::operator==(const Doctor& other) const { return fio == other.fio; }

bool Doctor::operator<(const Doctor& other) const { return experience < other.experience; }
bool Doctor::operator>(const Doctor& other) const { return experience > other.experience; }

std::ostream& operator<<(std::ostream& os, const Doctor& doc) {
    os << "Doctor #" << doc.id << " | Name: " << doc.fio
        << " | Specialty: " << doc.specialty << " | Experience: " << doc.experience << " years";
    return os;
}

std::istream& operator>>(std::istream& is, Doctor& doc) {
    std::cout << "Enter Doctor Name: ";
    std::getline(is >> std::ws, doc.fio);

    std::cout << "Enter Specialty: ";
    std::getline(is >> std::ws, doc.specialty);

    std::cout << "Enter Experience (years): ";
    is >> doc.experience;
    if (doc.experience < 0) {
        std::cout << "[ERROR] Invalid experience! Setting to 0.\n";
        doc.experience = 0;
    }
    return is;
}
