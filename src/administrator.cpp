#include "administrator.h"
#include "exceptions/invalid_data_exception.h"

int Administrator::counter = 0;

Administrator::Administrator(std::string_view name, std::string_view position,
    int experienceValue, int patientsPerDayValue)
    : Person(name, counter), position(position), experience(0), patientsPerDay(0) {
    setExperience(experienceValue);
    setPatientsPerDay(patientsPerDayValue);
}

std::string Administrator::getPosition() const { return position; }
int Administrator::getExperience() const { return experience; }
int Administrator::getPatientsPerDay() const { return patientsPerDay; }

void Administrator::setPosition(std::string_view positionValue) { position = positionValue; }
void Administrator::setExperience(int expValue) {
    if (expValue < 0) {
        throw InvalidDataException("experience", "must not be negative, got " + std::to_string(expValue));
    }
    experience = expValue;
}
void Administrator::setPatientsPerDay(int countValue) {
    if (countValue < 0) {
        throw InvalidDataException("patients per day", "must not be negative, got " + std::to_string(countValue));
    }
    patientsPerDay = countValue;
}

bool Administrator::operator==(const Administrator& other) const { return name == other.name; }
bool Administrator::operator<(const Administrator& other) const { return experience < other.experience; }
bool Administrator::operator>(const Administrator& other) const { return experience > other.experience; }

std::string Administrator::getEntityType() const { return "Administrator"; }

void Administrator::printInfo(std::ostream& os) const {
    os << getEntityType() << " ";
    Person::printInfo(os);
    os << " | Position: " << position << " | Experience: " << experience
        << " years | Patients/day: " << patientsPerDay;
}

std::string Administrator::classify() const {
    if (patientsPerDay >= 40) return "High workload";
    if (patientsPerDay >= 20) return "Normal workload";
    return "Low workload";
}

std::istream& operator>>(std::istream& is, Administrator& admin) {
    std::string nameValue;
    std::string positionValue;
    int experienceValue = 0;
    int patientsPerDayValue = 0;

    std::cout << "Enter Administrator Name: ";
    std::getline(is >> std::ws, nameValue);

    std::cout << "Enter Position: ";
    std::getline(is >> std::ws, positionValue);

    std::cout << "Enter Experience (years): ";
    is >> experienceValue;
    if (!is) return is;

    std::cout << "Enter Patients per day: ";
    is >> patientsPerDayValue;
    if (!is) return is;

    Administrator updated(admin);
    updated.setName(nameValue);
    updated.setPosition(positionValue);
    updated.setExperience(experienceValue);
    updated.setPatientsPerDay(patientsPerDayValue);
    admin = updated;
    return is;
}