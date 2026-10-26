#include "doctor.h"
#include "exceptions/invalid_data_exception.h"

int Doctor::counter = 0;

Doctor::Doctor(std::string_view name, std::string_view specialty, int experienceValue)
    : Doctor(counter, name, specialty, experienceValue) {
}

Doctor::Doctor(int id, std::string_view name, std::string_view specialty, int experienceValue)
    : Person(id, name, counter), specialty(specialty), experience(0) {
    setExperience(experienceValue);
}

std::string Doctor::getSpecialty() const { return specialty; }
int Doctor::getExperience() const { return experience; }

void Doctor::setSpecialty(std::string_view specialtyValue) { specialty = specialtyValue; }
void Doctor::setExperience(int expValue) {
    if (expValue < 0) {
        throw InvalidDataException("experience", "must not be negative, got " + std::to_string(expValue));
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
    std::string specialtyValue;
    int experienceValue = 0;

    std::cout << "Enter Doctor Name: ";
    std::getline(is >> std::ws, nameValue);

    std::cout << "Enter Specialty: ";
    std::getline(is >> std::ws, specialtyValue);

    std::cout << "Enter Experience (years): ";
    is >> experienceValue;
    if (!is) return is;

    Doctor updated(doc);
    updated.setName(nameValue);
    updated.setSpecialty(specialtyValue);
    updated.setExperience(experienceValue);
    doc = updated;
    return is;
}