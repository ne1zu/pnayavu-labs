#include "doctor.h"

int Doctor::counter = 0;

Doctor::Doctor(std::string_view name, std::string_view specialty, int experience)
    : Person(name, counter), specialty(specialty), experience(experience) {
}

std::string Doctor::getSpecialty() const { return specialty; }
int Doctor::getExperience() const { return experience; }

void Doctor::setSpecialty(std::string_view specialtyValue) { specialty = specialtyValue; }
void Doctor::setExperience(int expValue) {
    if (expValue < 0) {
        std::cout << "[ERROR] Invalid experience!\n";
        return;
    }
    experience = expValue;
}

bool Doctor::operator==(const Doctor& other) const { return name == other.name; }
bool Doctor::operator<(const Doctor& other) const { return experience < other.experience; }
bool Doctor::operator>(const Doctor& other) const { return experience > other.experience; }

std::string Doctor::getEntityType() const { return "Doctor"; }

void Doctor::printInfo(std::ostream& os) const {
    os << getEntityType() << " ";
    Person::printInfo(os);
    os << " | Specialty: " << specialty << " | Experience: " << experience << " years";
}

std::string Doctor::classify() const {
    if (experience >= 10) return "Senior";
    if (experience >= 5) return "Mid-level";
    return "Junior";
}

std::istream& operator>>(std::istream& is, Doctor& doc) {
    std::string nameValue;
    std::cout << "Enter Doctor Name: ";
    std::getline(is >> std::ws, nameValue);
    doc.setName(nameValue);

    std::cout << "Enter Specialty: ";
    std::getline(is >> std::ws, doc.specialty);

    std::cout << "Enter Experience (years): ";
    is >> doc.experience;
    if (doc.experience < 0) doc.experience = 0;
    return is;
}