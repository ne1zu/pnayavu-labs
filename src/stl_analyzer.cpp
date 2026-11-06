#include "stl_analyzer.h"
#include <numeric>
#include <set>
#include "exceptions/exceptions.h"

using std::shared_ptr;
using std::string;
using std::vector;

Statistics makeStatistics(const vector<double>& values) {
    Statistics result = { 0, 0.0, 0.0, 0.0, 0.0 };
    if (values.empty()) return result;
    result.count = static_cast<int>(values.size());
    result.sum = std::accumulate(values.begin(), values.end(), 0.0);
    result.min = *std::min_element(values.begin(), values.end());
    result.max = *std::max_element(values.begin(), values.end());
    result.average = result.sum / result.count;
    return result;
}

StlAnalyzer::StlAnalyzer(const ClinicData& data) {
    std::copy(data.doctors.begin(), data.doctors.end(), std::back_inserter(doctors));
    std::copy(data.patients.begin(), data.patients.end(), std::back_inserter(patients));
    std::copy(data.services.begin(), data.services.end(), std::back_inserter(services));
    for (const auto& filial : data.filials) {
        Collection<Appointment> branchAppointments = filial->getAppointments();
        std::copy(branchAppointments.begin(), branchAppointments.end(), std::back_inserter(appointments));
    }
    for (const auto& doctor : doctors) indexDoctor(doctor);
}

void StlAnalyzer::indexDoctor(const shared_ptr<Doctor>& doctor) {
    doctorById[doctor->getId()] = doctor;
    doctorsBySpecialty[doctor->getSpecialty()].push_back(doctor);
}

const vector<shared_ptr<Doctor>>& StlAnalyzer::getDoctors() const { return doctors; }
const vector<shared_ptr<Patient>>& StlAnalyzer::getPatients() const { return patients; }
const vector<shared_ptr<Service>>& StlAnalyzer::getServices() const { return services; }
const std::list<shared_ptr<Appointment>>& StlAnalyzer::getAppointments() const { return appointments; }
const std::map<string, vector<shared_ptr<Doctor>>>& StlAnalyzer::getDoctorsBySpecialty() const {
    return doctorsBySpecialty;
}

void StlAnalyzer::addDoctor(shared_ptr<Doctor> doctor) {
    if (!doctor) throw NullObjectException("STL workspace", "Doctor");
    auto hasSameName = [&doctor](const shared_ptr<Doctor>& other) { return *other == *doctor; };
    if (std::find_if(doctors.begin(), doctors.end(), hasSameName) != doctors.end()
        || doctorById.count(doctor->getId()) > 0) {
        throw DuplicateObjectException("STL workspace", "Doctor");
    }
    doctors.push_back(doctor);
    indexDoctor(doctor);
}

void StlAnalyzer::removeDoctor(int id) {
    auto found = doctorById.find(id);
    if (found == doctorById.end()) throw ObjectNotFoundException("STL workspace", "Doctor");

    shared_ptr<Doctor> doctor = found->second;
    const string specialty = doctor->getSpecialty();

    doctors.erase(std::find(doctors.begin(), doctors.end(), doctor));
    vector<shared_ptr<Doctor>>& group = doctorsBySpecialty[specialty];
    group.erase(std::remove(group.begin(), group.end(), doctor), group.end());
    if (group.empty()) doctorsBySpecialty.erase(specialty);
    doctorById.erase(found);

    appointments.remove_if([id](const shared_ptr<Appointment>& appointment) {
        return appointment->getDoctor()->getId() == id;
    });
}

void StlAnalyzer::addPatient(shared_ptr<Patient> patient) {
    if (!patient) throw NullObjectException("STL workspace", "Patient");
    auto hasSamePhone = [&patient](const shared_ptr<Patient>& other) { return *other == *patient; };
    if (std::find_if(patients.begin(), patients.end(), hasSamePhone) != patients.end()) {
        throw DuplicateObjectException("STL workspace", "Patient");
    }
    patients.push_back(patient);
}

void StlAnalyzer::removePatient(int id) {
    auto hasThisId = [id](const shared_ptr<Patient>& patient) { return patient->getId() == id; };
    auto newEnd = std::remove_if(patients.begin(), patients.end(), hasThisId);
    if (newEnd == patients.end()) throw ObjectNotFoundException("STL workspace", "Patient");
    patients.erase(newEnd, patients.end());

    appointments.remove_if([id](const shared_ptr<Appointment>& appointment) {
        return appointment->getPatient()->getId() == id;
    });
}

void StlAnalyzer::addService(shared_ptr<Service> service) {
    if (!service) throw NullObjectException("STL workspace", "Service");
    auto hasSameName = [&service](const shared_ptr<Service>& other) { return *other == *service; };
    if (std::find_if(services.begin(), services.end(), hasSameName) != services.end()) {
        throw DuplicateObjectException("STL workspace", "Service");
    }
    services.push_back(service);
}

void StlAnalyzer::removeService(int id) {
    auto hasThisId = [id](const shared_ptr<Service>& service) { return service->getId() == id; };
    auto newEnd = std::remove_if(services.begin(), services.end(), hasThisId);
    if (newEnd == services.end()) throw ObjectNotFoundException("STL workspace", "Service");
    services.erase(newEnd, services.end());

    appointments.remove_if([id](const shared_ptr<Appointment>& appointment) {
        return appointment->getService()->getId() == id;
    });
}
void StlAnalyzer::addAppointment(shared_ptr<Appointment> appointment) {
    if (!appointment) throw NullObjectException("STL workspace", "Appointment");
    for (const auto& a : appointments) {
        if (a->getId() == appointment->getId()) {
            throw DuplicateObjectException("STL workspace", "Appointment");
        }
    }
    appointments.push_back(appointment);
}
void StlAnalyzer::cancelAppointment(int id) {
    std::size_t before = appointments.size();
    appointments.remove_if([id](const shared_ptr<Appointment>& appointment) { return appointment->getId() == id; });
    if (appointments.size() == before) throw ObjectNotFoundException("STL workspace", "Appointment");
}

shared_ptr<Doctor> StlAnalyzer::findDoctorById(int id) const {
    auto found = doctorById.find(id);
    if (found == doctorById.end()) return nullptr;
    return found->second;
}

shared_ptr<Doctor> StlAnalyzer::findDoctorByName(const string& name) const {
    auto found = std::find_if(doctors.begin(), doctors.end(),
        [&name](const shared_ptr<Doctor>& doctor) { return doctor->getName() == name; });
    if (found == doctors.end()) return nullptr;
    return *found;
}

vector<shared_ptr<Doctor>> StlAnalyzer::findDoctors(const string& specialty, int minExperience) const {
    vector<shared_ptr<Doctor>> result;
    std::copy_if(doctors.begin(), doctors.end(), std::back_inserter(result),
        [&specialty, minExperience](const shared_ptr<Doctor>& doctor) {
            bool specialtyMatches = specialty.empty() || doctor->getSpecialty() == specialty;
            return specialtyMatches && doctor->getExperience() >= minExperience;
        });
    return result;
}

vector<shared_ptr<Patient>> StlAnalyzer::findPatients(int minAge, int maxAge) const {
    vector<shared_ptr<Patient>> result;
    std::copy_if(patients.begin(), patients.end(), std::back_inserter(result),
        [minAge, maxAge](const shared_ptr<Patient>& patient) {
            return patient->getAge() >= minAge && patient->getAge() <= maxAge;
        });
    return result;
}

vector<shared_ptr<Service>> StlAnalyzer::findServices(int minPrice, int maxPrice) const {
    vector<shared_ptr<Service>> result;
    std::copy_if(services.begin(), services.end(), std::back_inserter(result),
        [minPrice, maxPrice](const shared_ptr<Service>& service) {
            return service->getPrice() >= minPrice && service->getPrice() <= maxPrice;
        });
    return result;
}

vector<shared_ptr<Appointment>> StlAnalyzer::findAppointments(int doctorId, int month) const {
    vector<shared_ptr<Appointment>> result;
    std::copy_if(appointments.begin(), appointments.end(), std::back_inserter(result),
        [doctorId, month](const shared_ptr<Appointment>& appointment) {
            bool doctorMatches = doctorId < 0 || appointment->getDoctor()->getId() == doctorId;
            bool monthMatches = month == 0 || appointment->getMonth() == month;
            return doctorMatches && monthMatches;
        });
    return result;
}

vector<shared_ptr<Doctor>> StlAnalyzer::sortedDoctors(bool byName, bool ascending) const {
    vector<shared_ptr<Doctor>> result(doctors);
    std::sort(result.begin(), result.end(),
        [byName, ascending](const shared_ptr<Doctor>& a, const shared_ptr<Doctor>& b) {
            if (byName) return ascending ? a->getName() < b->getName() : a->getName() > b->getName();
            return ascending ? *a < *b : *a > *b;
        });
    return result;
}

vector<shared_ptr<Patient>> StlAnalyzer::sortedPatients(bool ascending) const {
    vector<shared_ptr<Patient>> result(patients);
    std::sort(result.begin(), result.end(), [ascending](const shared_ptr<Patient>& a, const shared_ptr<Patient>& b) {
        return ascending ? *a < *b : *a > *b;
    });
    return result;
}

vector<shared_ptr<Service>> StlAnalyzer::sortedServices(bool ascending) const {
    vector<shared_ptr<Service>> result(services);
    std::sort(result.begin(), result.end(), [ascending](const shared_ptr<Service>& a, const shared_ptr<Service>& b) {
        return ascending ? *a < *b : *a > *b;
    });
    return result;
}

std::list<shared_ptr<Appointment>> StlAnalyzer::sortedAppointments(bool ascending) const {
    std::list<shared_ptr<Appointment>> result(appointments);
    result.sort([ascending](const shared_ptr<Appointment>& a, const shared_ptr<Appointment>& b) {
        return ascending ? *a < *b : *a > *b;
    });
    return result;
}

int StlAnalyzer::countDoctors(int minExperience) const {
    return static_cast<int>(std::count_if(doctors.begin(), doctors.end(),
        [minExperience](const shared_ptr<Doctor>& doctor) { return doctor->getExperience() >= minExperience; }));
}

int StlAnalyzer::countAdultPatients() const {
    return static_cast<int>(std::count_if(patients.begin(), patients.end(),
        [](const shared_ptr<Patient>& patient) { return patient->isAdult(); }));
}

int StlAnalyzer::countServicesCheaperThan(int price) const {
    return static_cast<int>(std::count_if(services.begin(), services.end(),
        [price](const shared_ptr<Service>& service) { return service->getPrice() < price; }));
}

int StlAnalyzer::countMorningAppointments() const {
    return static_cast<int>(std::count_if(appointments.begin(), appointments.end(),
        [](const shared_ptr<Appointment>& appointment) { return appointment->isMorning(); }));
}

int StlAnalyzer::countAppointmentsOfDoctor(int doctorId) const {
    vector<int> doctorIds;
    std::transform(appointments.begin(), appointments.end(), std::back_inserter(doctorIds),
        [](const shared_ptr<Appointment>& appointment) { return appointment->getDoctor()->getId(); });
    return static_cast<int>(std::count(doctorIds.begin(), doctorIds.end(), doctorId));
}

std::map<string, int> StlAnalyzer::countAppointmentsByDoctor() const {
    std::map<string, int> result;
    std::for_each(appointments.begin(), appointments.end(),
        [&result](const shared_ptr<Appointment>& appointment) { result[appointment->getDoctor()->getName()]++; });
    return result;
}

std::pair<string, int> StlAnalyzer::findBusiestDoctor() const {
    std::map<string, int> counts = countAppointmentsByDoctor();
    if (counts.empty()) return std::make_pair(string(), 0);
    auto busiest = std::max_element(counts.begin(), counts.end(),
        [](const std::pair<const string, int>& a, const std::pair<const string, int>& b) {
            return a.second < b.second;
        });
    return std::make_pair(busiest->first, busiest->second);
}

Statistics StlAnalyzer::doctorExperienceStatistics() const {
    return makeStatistics(collectValues(doctors,
        [](const shared_ptr<Doctor>& doctor) { return static_cast<double>(doctor->getExperience()); }));
}

Statistics StlAnalyzer::patientAgeStatistics() const {
    return makeStatistics(collectValues(patients,
        [](const shared_ptr<Patient>& patient) { return static_cast<double>(patient->getAge()); }));
}

Statistics StlAnalyzer::servicePriceStatistics() const {
    return makeStatistics(collectValues(services,
        [](const shared_ptr<Service>& service) { return static_cast<double>(service->getPrice()); }));
}

int StlAnalyzer::getUniquePatientCount() const {
    std::set<int> patientIds;
    for (const auto& appointment : appointments) patientIds.insert(appointment->getPatient()->getId());
    return static_cast<int>(patientIds.size());
}

double StlAnalyzer::getTotalRevenue() const {
    return std::accumulate(appointments.begin(), appointments.end(), 0.0,
        [](double sum, const shared_ptr<Appointment>& appointment) {
            return sum + appointment->getService()->getPrice();
        });
}

int StlAnalyzer::getTotalMinutes() const {
    return std::accumulate(appointments.begin(), appointments.end(), 0,
        [](int sum, const shared_ptr<Appointment>& appointment) {
            return sum + appointment->getService()->getLength();
        });
}
