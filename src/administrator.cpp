#include "administrator.h"

int Administrator::counter = 0;

Administrator::Administrator(std::string_view fio, std::string_view position,
    int experience, int patientsPerDay)
    : Person(fio, counter), position(position),
      experience(experience), patientsPerDay(patientsPerDay) {
}

std::string Administrator::getFio() const { return getName(); }
std::string Administrator::getPosition() const { return position; }
int Administrator::getExperience() const { return experience; }
int Administrator::getPatientsPerDay() const { return patientsPerDay; }

void Administrator::setFio(std::string_view fioValue) { setName(fioValue); }
void Administrator::setPosition(std::string_view positionValue) { position = positionValue; }
void Administrator::setExperience(int expValue) {
    if (expValue < 0) {
        std::cout << "[ERROR] Experience cannot be negative!\n";
        return;
    }
    experience = expValue;
}
void Administrator::setPatientsPerDay(int countValue) {
    if (countValue < 0) {
        std::cout << "[ERROR] Patients per day cannot be negative!\n";
        return;
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
    std::string fioValue;
    std::cout << "Enter Administrator Name: ";
    std::getline(is >> std::ws, fioValue);
    admin.setFio(fioValue);

    std::cout << "Enter Position: ";
    std::getline(is >> std::ws, admin.position);

    std::cout << "Enter Experience (years): ";
    is >> admin.experience;
    if (admin.experience < 0) {
        std::cout << "[ERROR] Invalid experience! Setting to 0.\n";
        admin.experience = 0;
    }

    std::cout << "Enter Patients per day: ";
    is >> admin.patientsPerDay;
    if (admin.patientsPerDay < 0) {
        std::cout << "[ERROR] Invalid patients per day! Setting to 0.\n";
        admin.patientsPerDay = 0;
    }
    return is;
}
