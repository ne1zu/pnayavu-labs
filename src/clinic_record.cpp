#include "clinic_record.h"

ClinicRecord::ClinicRecord(std::string_view name, int& typeCounter)
    : name(name) ,id(typeCounter++) {
}

int ClinicRecord::getId() const { return id; }
std::string ClinicRecord::getName() const { return name; }
void ClinicRecord::setName(std::string_view nameValue) { name = nameValue; }

void ClinicRecord::printInfo(std::ostream& os) const {
    os << "#" << id << " | Name: " << name;
}

std::ostream& operator<<(std::ostream& os, const ClinicRecord& entity) {
    entity.printInfo(os);
    return os;
}
