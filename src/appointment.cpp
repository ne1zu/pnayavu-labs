#include "appointment.h"
#include <sstream>
#include <iomanip>

int Appointment::counter = 0;

Appointment::Appointment(std::shared_ptr<Patient> patient, std::shared_ptr<Doctor> doctor,
    std::shared_ptr<Service> service, std::string_view filialName,
    int day, int month, int year, int hour, int minute)
    : id(counter++), patient(patient), doctor(doctor), service(service), filialName(filialName),
    day(day), month(month), year(year), hour(hour), minute(minute) {
}

int Appointment::getId() const { return id; }
std::shared_ptr<Patient> Appointment::getPatient() const { return patient; }
std::shared_ptr<Doctor> Appointment::getDoctor() const { return doctor; }
std::shared_ptr<Service> Appointment::getService() const { return service; }
std::string Appointment::getFilialName() const { return filialName; }
int Appointment::getDay() const { return day; }
int Appointment::getMonth() const { return month; }
int Appointment::getYear() const { return year; }
int Appointment::getHour() const { return hour; }
int Appointment::getMinute() const { return minute; }

void Appointment::setFilialName(std::string_view filialNameValue) { filialName = filialNameValue; }

bool Appointment::isValidDate(int day, int month, int year) {
    if (year < 2000 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    int limit = daysInMonth[month - 1];
    if (month == 2 && leap) limit = 29;
    return day >= 1 && day <= limit;
}

bool Appointment::isValidTime(int hour, int minute) {
    return hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59;
}

bool Appointment::isComplete() const {
    return patient && doctor && service;
}

bool Appointment::hasValidDateTime() const {
    return isValidDate(day, month, year) && isValidTime(hour, minute);
}

int Appointment::getStartMinutes() const { return hour * 60 + minute; }

int Appointment::getEndMinutes() const {
    int length = service ? service->getLength() : 0;
    return getStartMinutes() + length;
}

bool Appointment::fitsWorkingHours() const {
    return getStartMinutes() >= WORK_START && getEndMinutes() <= WORK_END;
}

bool Appointment::overlaps(const Appointment& other) const {
    bool sameDay = day == other.day && month == other.month && year == other.year;
    if (!sameDay) return false;
    return getStartMinutes() < other.getEndMinutes() && other.getStartMinutes() < getEndMinutes();
}

long long Appointment::timeKey() const {
    return (((static_cast<long long>(year) * 13 + month) * 32 + day) * 1440) + getStartMinutes();
}

std::string Appointment::getDateTimeString() const {
    int end = getEndMinutes();
    std::ostringstream out;
    out << std::setfill('0')
        << std::setw(2) << day << "." << std::setw(2) << month << "." << year << " "
        << std::setw(2) << hour << ":" << std::setw(2) << minute << "-"
        << std::setw(2) << end / 60 << ":" << std::setw(2) << end % 60;
    return out.str();
}

bool Appointment::operator==(const Appointment& other) const {
    if (!isComplete() || !other.isComplete()) return false;
    return *patient == *other.patient && *doctor == *other.doctor && *service == *other.service
        && day == other.day && month == other.month && year == other.year
        && hour == other.hour && minute == other.minute;
}
bool Appointment::operator<(const Appointment& other) const { return timeKey() < other.timeKey(); }
bool Appointment::operator>(const Appointment& other) const { return timeKey() > other.timeKey(); }

std::string Appointment::getEntityType() const { return "Appointment"; }

void Appointment::printInfo(std::ostream& os) const {
    os << getEntityType() << " #" << id;
    os << " | Patient: " << (patient ? patient->getName() : "-");
    os << " | Doctor: " << (doctor ? doctor->getName() : "-");
    os << " | Service: " << (service ? service->getName() : "-");
    os << " | Filial: " << filialName;
    os << " | Time: " << getDateTimeString();
}

std::string Appointment::classify() const {
    if (isMorning()) return "Morning";
    if (getStartMinutes() < 17 * 60) return "Afternoon";
    return "Evening";
}

std::ostream& operator<<(std::ostream& os, const Appointment& appointment) {
    appointment.printInfo(os);
    return os;
}