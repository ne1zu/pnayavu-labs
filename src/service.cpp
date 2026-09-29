#include "service.h"

int Service::counter = 0;

Service::Service(std::string_view name, float price, int length)
    : name(name), price(price), length(length) {
    id = counter++;
}

int Service::getId() const { return id; }
std::string Service::getName() const { return name; }
float Service::getPrice() const { return price; }
int Service::getLength() const { return length; }

void Service::setName(std::string_view nameValue) { name = nameValue; }
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

std::ostream& operator<<(std::ostream& os, const Service& srv) {
    os << "Service #" << srv.id << " | Name: " << srv.name
        << " | Price: " << srv.price << "$ | Duration: " << srv.length << " min";
    return os;
}

std::istream& operator>>(std::istream& is, Service& srv) {
    std::cout << "Enter Service Name: ";
    std::getline(is >> std::ws, srv.name);

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
