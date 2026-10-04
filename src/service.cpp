#include "service.h"

int Service::counter = 0;

Service::Service(std::string_view name, float price, int length)
    : ClinicRecord(name, counter), price(price), length(length) {
}

float Service::getPrice() const { return price; }
int Service::getLength() const { return length; }

void Service::setPrice(float priceValue) {
    if (priceValue < 0) {
        std::cout << "[ERROR] Price cannot be negative.\n";
        return;
    }
    price = priceValue;
}
void Service::setLength(int lengthValue) {
    if (lengthValue <= 0) {
        std::cout << " Duration must be positive.\n";
        return;
    }
    length = lengthValue;
}

bool Service::operator==(const Service& other) const { return name == other.name; }
bool Service::operator<(const Service& other) const { return price < other.price; }
bool Service::operator>(const Service& other) const { return price > other.price; }

std::string Service::getEntityType() const { return "Service"; }

void Service::printInfo(std::ostream& os) const {
    os << getEntityType() << " ";
    ClinicRecord::printInfo(os);
    os << " | Price: " << price << "$ | Duration: " << length << " min";
}

std::string Service::classify() const {
    return price >= 40.0f ? "Premium" : "Standard";
}

std::istream& operator>>(std::istream& is, Service& srv) {
    std::string nameValue;
    std::cout << "Enter Service Name: ";
    std::getline(is >> std::ws, nameValue);
    srv.setName(nameValue);

    std::cout << "Enter Price ($): ";
    is >> srv.price;
    if (srv.price < 0) {
        std::cout << " Invalid price! Setting to 0.\n";
        srv.price = 0;
    }

    std::cout << "Enter Duration (min): ";
    is >> srv.length;
    if (srv.length <= 0) {
        std::cout << " Invalid duration! Setting to 10 min.\n";
        srv.length = 10;
    }
    return is;
}
