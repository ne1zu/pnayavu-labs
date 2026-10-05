#include "patient.h"

int Patient::counter = 0;

Patient::Patient(std::string_view fio, int age, std::string_view phone)
    : Person(fio, counter), age(age), phone(phone) {
}

std::string Patient::getFio() const { return getName(); }
int Patient::getAge() const { return age; }
std::string Patient::getPhone() const { return phone; }

void Patient::setFio(std::string_view fioValue) { setName(fioValue); }
void Patient::setAge(int ageValue) {
    if (ageValue < 0) {
        std::cout << " Age cannot be negative!\n";
        return;
    }
    age = ageValue;
}
void Patient::setPhone(std::string_view phoneValue) { phone = phoneValue; }

bool Patient::isAdult() const { return age >= 18; }

bool Patient::operator==(const Patient& other) const { return phone == other.phone; }
bool Patient::operator<(const Patient& other) const { return age < other.age; }
bool Patient::operator>(const Patient& other) const { return age > other.age; }

std::string Patient::getEntityType() const { return "Patient"; }

void Patient::printInfo(std::ostream& os) const {
    os << getEntityType() << " ";
    Person::printInfo(os);
    os << " | Age: " << age << " | Phone: " << phone;
}

std::string Patient::classify() const {
    return isAdult() ? "Adult" : "Minor";
}

std::istream& operator>>(std::istream& is, Patient& patient) {
    std::string fioValue;
    std::cout << "Enter Patient Name: ";
    std::getline(is >> std::ws, fioValue);
    patient.setFio(fioValue);

    std::cout << "Enter Patient Age: ";
    is >> patient.age;
    if (patient.age < 0) {
        std::cout << "[ERROR] Invalid age! Setting to 0.\n";
        patient.age = 0;
    }

    std::cout << "Enter Patient Phone: ";
    std::getline(is >> std::ws, patient.phone);
    return is;
}
