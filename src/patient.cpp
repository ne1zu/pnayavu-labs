#include "patient.h"
#include "exceptions/invalid_data_exception.h"

int Patient::counter = 0;

Patient::Patient(std::string_view name, int ageValue, std::string_view phone)
    : Person(name, counter), age(0), phone(phone) {
    setAge(ageValue);
}

int Patient::getAge() const { return age; }
std::string Patient::getPhone() const { return phone; }

void Patient::setAge(int ageValue) {
    if (ageValue < 0) {
        throw InvalidDataException("age", "must not be negative, got " + std::to_string(ageValue));
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
    std::string phoneValue;
    int ageValue = 0;

    std::cout << "Enter Patient Name: ";
    std::getline(is >> std::ws, nameValue);

    std::cout << "Enter Patient Age: ";
    is >> ageValue;
    if (!is) return is;

    std::cout << "Enter Patient Phone: ";
    std::getline(is >> std::ws, phoneValue);
    if (!is) return is;

    Patient updated(patient);
    updated.setName(nameValue);
    updated.setAge(ageValue);
    updated.setPhone(phoneValue);
    patient = updated;
    return is;
}