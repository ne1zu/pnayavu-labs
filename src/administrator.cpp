#include "administrator.h"

int Administrator::counter = 0;

Administrator::Administrator(std::string_view name, std::string_view position, int managedPeople)
    : Person(name, counter), position(position), managedPeople(managedPeople) {
}

std::string Administrator::getPosition() const { return position; }
int Administrator::getManagedPeople() const { return managedPeople; }

void Administrator::setPosition(std::string_view posValue) { position = posValue; }
void Administrator::setManagedPeople(int count) {
    if (count < 0) {
        std::cout << "[ERROR] Number of managed people cannot be negative!\n";
        return;
    }
    managedPeople = count;
}

bool Administrator::operator==(const Administrator& other) const { return name == other.name; }
bool Administrator::operator<(const Administrator& other) const { return managedPeople < other.managedPeople; }
bool Administrator::operator>(const Administrator& other) const { return managedPeople > other.managedPeople; }

std::string Administrator::getEntityType() const { return "Administrator"; }

void Administrator::printInfo(std::ostream& os) const {
    os << getEntityType() << " ";
    Person::printInfo(os);
    os << " | Position: " << position << " | Subordinates: " << managedPeople;
}

std::string Administrator::classify() const {
    return managedPeople >= 10 ? "Top-Manager" : "Line-Manager";
}

std::istream& operator>>(std::istream& is, Administrator& admin) {
    std::string nameValue;
    std::cout << "Enter Admin Name: ";
    std::getline(is >> std::ws, nameValue);
    admin.setName(nameValue);

    std::cout << "Enter Position (e.g. Chief, HR): ";
    std::getline(is >> std::ws, admin.position);

    std::cout << "Enter number of subordinates: ";
    is >> admin.managedPeople;
    if (admin.managedPeople < 0) {
        std::cout << "[ERROR] Invalid amount! Setting to 0.\n";
        admin.managedPeople = 0;
    }
    return is;
}