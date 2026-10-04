#include "doctor.h"

int Doctor::counter = 0;

Doctor::Doctor(std::string_view fio, std::string_view specialty, int experience)
    : ClinicRecord(fio, counter), specialty(specialty), experience(experience) {
}

std::string Doctor::getFio() const { return getName(); }
std::string Doctor::getSpecialty() const { return specialty; }
int Doctor::getExperience() const { return experience; }

void Doctor::setFio(std::string_view fioValue) { setName(fioValue); }
void Doctor::setSpecialty(std::string_view specialtyValue) { specialty = specialtyValue; }
void Doctor::setExperience(int expValue) {
    if (expValue < 0) {
        std::cout << "[ERROR] Experience cannot be negative!\n";
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
    ClinicRecord::printInfo(os);
    os << " | Specialty: " << specialty << " | Experience: " << experience << " years";
}

std::string Doctor::classify() const {
    if (experience >= 10) return "Senior";
    if (experience >= 5) return "Mid-level";
    return "Junior";
}

std::istream& operator>>(std::istream& is, Doctor& doc) {
    std::string fioValue;
    std::cout << "Enter Doctor Name: ";
    std::getline(is >> std::ws, fioValue);
    doc.setFio(fioValue);

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
