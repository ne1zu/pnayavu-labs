#pragma once
#include <algorithm>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "doctor.h"
#include "patient.h"
#include "service.h"
#include "appointment.h"
#include "administrator.h"
#include "clinic_data.h"


struct Statistics {
    int count;
    double sum;
    double min;
    double max;
    double average;
};

Statistics makeStatistics(const std::vector<double>& values);

template <typename Container>
std::pair<typename Container::value_type, typename Container::value_type> findExtremes(const Container& items) {
    typedef typename Container::value_type Pointer;
    if (items.empty()) return std::make_pair(Pointer(), Pointer());
    auto isLess = [](const Pointer& a, const Pointer& b) { return *a < *b; };
    Pointer smallest = *std::min_element(items.begin(), items.end(), isLess);
    Pointer biggest = *std::max_element(items.begin(), items.end(), isLess);
    return std::make_pair(smallest, biggest);
}

template <typename Container>
std::map<std::string, std::vector<typename Container::value_type>> groupByClassification(const Container& items) {
    typedef typename Container::value_type Pointer;
    std::map<std::string, std::vector<Pointer>> groups;
    std::for_each(items.begin(), items.end(),
        [&groups](const Pointer& item) { groups[item->classify()].push_back(item); });
    return groups;
}

template <typename Container, typename Getter>
std::vector<double> collectValues(const Container& items, Getter getter) {
    std::vector<double> values;
    std::transform(items.begin(), items.end(), std::back_inserter(values), getter);
    return values;
}

class StlAnalyzer {
private:
    std::vector<std::shared_ptr<Doctor>> doctors;
    std::vector<std::shared_ptr<Patient>> patients;
    std::vector<std::shared_ptr<Service>> services;
    std::list<std::shared_ptr<Appointment>> appointments;

    std::unordered_map<int, std::shared_ptr<Doctor>> doctorById;
    std::map<std::string, std::vector<std::shared_ptr<Doctor>>> doctorsBySpecialty;

    void indexDoctor(const std::shared_ptr<Doctor>& doctor);

public:
    explicit StlAnalyzer(const ClinicData& data);

    const std::vector<std::shared_ptr<Doctor>>& getDoctors() const;
    const std::vector<std::shared_ptr<Patient>>& getPatients() const;
    const std::vector<std::shared_ptr<Service>>& getServices() const;
    const std::list<std::shared_ptr<Appointment>>& getAppointments() const;
    const std::map<std::string, std::vector<std::shared_ptr<Doctor>>>& getDoctorsBySpecialty() const;

    void addDoctor(std::shared_ptr<Doctor> doctor);
    void removeDoctor(int id);
    void addPatient(std::shared_ptr<Patient> patient);
    void removePatient(int id);
    void addService(std::shared_ptr<Service> service);
    void removeService(int id);
    void cancelAppointment(int id);

    std::shared_ptr<Doctor> findDoctorById(int id) const;
    std::shared_ptr<Doctor> findDoctorByName(const std::string& name) const;
    std::vector<std::shared_ptr<Doctor>> findDoctors(const std::string& specialty, int minExperience) const;
    std::vector<std::shared_ptr<Patient>> findPatients(int minAge, int maxAge) const;
    std::vector<std::shared_ptr<Service>> findServices(int minPrice, int maxPrice) const;
    std::vector<std::shared_ptr<Appointment>> findAppointments(int doctorId, int month) const;

    std::vector<std::shared_ptr<Doctor>> sortedDoctors(bool byName, bool ascending) const;
    std::vector<std::shared_ptr<Patient>> sortedPatients(bool ascending) const;
    std::vector<std::shared_ptr<Service>> sortedServices(bool ascending) const;
    std::list<std::shared_ptr<Appointment>> sortedAppointments(bool ascending) const;

    int countDoctors(int minExperience) const;
    int countAdultPatients() const;
    int countServicesCheaperThan(int price) const;
    int countMorningAppointments() const;
    int countAppointmentsOfDoctor(int doctorId) const;


    std::map<std::string, int> countAppointmentsByDoctor() const;
    std::pair<std::string, int> findBusiestDoctor() const;

    Statistics doctorExperienceStatistics() const;
    Statistics patientAgeStatistics() const;
    Statistics servicePriceStatistics() const;
    int getUniquePatientCount() const;
    double getTotalRevenue() const;
    int getTotalMinutes() const;
};
