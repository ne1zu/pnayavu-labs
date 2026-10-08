#include "person.h"

Person::Person(std::string_view name, int& typeCounter)
    : name(name) {
    id = typeCounter++;
}

int Person::getId() const { return id; }
std::string Person::getName() const { return name; }
void Person::setName(std::string_view nameValue) { name = nameValue; }

void Person::printInfo(std::ostream& os) const {
    os << "#" << id << " | Name: " << name;
}

std::ostream& operator<<(std::ostream& os, const Person& entity) {
    entity.printInfo(os);
    return os;
}
