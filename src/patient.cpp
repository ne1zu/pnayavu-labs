#include "patient.h"

int Patient::counter = 0;

Patient::Patient(std::string_view fio, int age, std::string_view phone)
    : fio(fio), age(age), phone(phone) {
    id = counter++;
}

int Patient::getId() const { return id; }
std::string Patient::getFio() const { return fio; }
int Patient::getAge() const { return age; }
std::string Patient::getPhone() const { return phone; }

void Patient::setFio(std::string_view fioValue) { fio = fioValue; }
void Patient::setAge(int ageValue) {
    if (ageValue < 0) {
        std::cout << " Age cannot be negative!\n";
        return;
    }
    age = ageValue;
}
void Patient::setPhone(std::string_view phoneValue) { phone = phoneValue; }

bool Patient::isAdult() const { return age >= 18; }

bool Patient::operator==(const Patient& other) const { return id == other.id; }

bool Patient::operator<(const Patient& other) const { return age < other.age; }
bool Patient::operator>(const Patient& other) const { return age > other.age; }

std::ostream& operator<<(std::ostream& os, const Patient& patient) {
    os << "Patient #" << patient.id << " | Name: " << patient.fio
        << " | Age: " << patient.age << " | Phone: " << patient.phone;
    return os;
}

std::istream& operator>>(std::istream& is, Patient& patient) {
    std::cout << "Enter Patient Name: ";
    std::getline(is >> std::ws, patient.fio);
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
