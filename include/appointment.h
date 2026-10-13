#pragma once
#include <string>
#include <string_view>
#include <iostream>
#include <memory>
#include "patient.h"
#include "doctor.h"
#include "service.h"

class Appointment {
private:
    int id;
    std::shared_ptr<Patient> patient;
    std::shared_ptr<Doctor> doctor;
    std::shared_ptr<Service> service;
    std::string filialName;
    int day;
    int month;
    int year;
    int hour;
    int minute;
    static int counter;

    long long timeKey() const;

public:
    static const int WORK_START = 8 * 60;
    static const int WORK_END = 20 * 60;

    Appointment(std::shared_ptr<Patient> patient, std::shared_ptr<Doctor> doctor,
        std::shared_ptr<Service> service, std::string_view filialName,
        int day, int month, int year, int hour, int minute);

    int getId() const;
    std::shared_ptr<Patient> getPatient() const;
    std::shared_ptr<Doctor> getDoctor() const;
    std::shared_ptr<Service> getService() const;
    std::string getFilialName() const;
    int getDay() const;
    int getMonth() const;
    int getYear() const;
    int getHour() const;
    int getMinute() const;

    void setFilialName(std::string_view filialNameValue);

    static bool isValidDate(int day, int month, int year);
    static bool isValidTime(int hour, int minute);
    bool isComplete() const;
    bool hasValidDateTime() const;
    bool fitsWorkingHours() const;
    bool overlaps(const Appointment& other) const;

    int getStartMinutes() const;
    int getEndMinutes() const;
    std::string getDateTimeString() const;

    bool operator==(const Appointment& other) const;
    bool operator<(const Appointment& other) const;
    bool operator>(const Appointment& other) const;

    std::string getEntityType() const;
    void printInfo(std::ostream& os) const;
    std::string classify() const;

    friend std::ostream& operator<<(std::ostream& os, const Appointment& appointment);
};