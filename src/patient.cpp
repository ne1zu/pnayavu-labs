#include "patient.h"

int Patient::counter = 0;

Patient::Patient(std::string_view name, int age, std::string_view phone)
    : Person(name, counter), age(age), phone(phone) {
}

int Patient::getAge() const { return age; }
std::string Patient::getPhone() const { return phone; }

void Patient::setAge(int ageValue) {
    if (ageValue < 0) {
        std::cout << "[ERROR] Invalid age!\n";
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
    std::string nameValue;
    std::cout << "Enter Patient Name: ";
    std::getline(is >> std::ws, nameValue);
    patient.setName(nameValue);

    std::cout << "Enter Patient Age: ";
    is >> patient.age;
    if (patient.age < 0) patient.age = 0;

    std::cout << "Enter Patient Phone: ";
    std::getline(is >> std::ws, patient.phone);
    return is;
}