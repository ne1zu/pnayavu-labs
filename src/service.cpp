#include "service.h"
#include "exceptions/invalid_data_exception.h"

int Service::counter = 0;

Service::Service(std::string_view name, float priceValue, int lengthValue)
    : id(counter++), name(name), price(0.0f), length(0) {
    setPrice(priceValue);
    if (lengthValue < 0) {
        throw InvalidDataException("duration", "must not be negative, got " + std::to_string(lengthValue));
    }
    length = lengthValue;
}

int Service::getId() const { return id; }
std::string Service::getName() const { return name; }
float Service::getPrice() const { return price; }
int Service::getLength() const { return length; }

void Service::setName(std::string_view nameValue) { name = nameValue; }
void Service::setPrice(float priceValue) {
    if (priceValue < 0) {
        throw InvalidDataException("price", "must not be negative");
    }
    price = priceValue;
}
void Service::setLength(int lengthValue) {
    if (lengthValue <= 0) {
        throw InvalidDataException("duration", "must be positive, got " + std::to_string(lengthValue));
    }
    length = lengthValue;
}

bool Service::operator==(const Service& other) const { return name == other.name; }
bool Service::operator<(const Service& other) const { return price < other.price; }
bool Service::operator>(const Service& other) const { return price > other.price; }

std::string Service::getEntityType() const { return "Service"; }

void Service::printInfo(std::ostream& os) const {
    os << getEntityType() << " #" << id << " | Name: " << name;
    os << " | Price: " << price << "$ | Duration: " << length << " min";
}

std::string Service::classify() const {
    return price >= 40.0f ? "Premium" : "Standard";
}

std::ostream& operator<<(std::ostream& os, const Service& srv) {
    srv.printInfo(os);
    return os;
}

std::istream& operator>>(std::istream& is, Service& srv) {
    std::string nameValue;
    float priceValue = 0.0f;
    int lengthValue = 0;

    std::cout << "Enter Service Name: ";
    std::getline(is >> std::ws, nameValue);

    std::cout << "Enter Price ($): ";
    is >> priceValue;
    if (!is) return is;

    std::cout << "Enter Duration (min): ";
    is >> lengthValue;
    if (!is) return is;

    Service updated(srv);
    updated.setName(nameValue);
    updated.setPrice(priceValue);
    updated.setLength(lengthValue);
    srv = updated;
    return is;
}