#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <stdexcept>
#include <cstdio>
#include <filesystem>
#include <utility>
#include "service.h"
#include "doctor.h"
#include "patient.h"
#include "administrator.h"
#include "filial.h"
#include "person.h"
#include "collection.h"
#include "appointment.h"
#include "exceptions/exceptions.h"
#include "clinic_data.h"
#include "data_storage.h"
#include "logger.h"
#include "report_writer.h"

using namespace std;

const char* const STATE_FILE_NAME = "clinic_state.txt";
const char* const LOG_FILE_NAME = "clinic_log.txt";
const char* const REPORT_FILE_NAME = "clinic_report.txt";

Logger logger(LOG_FILE_NAME);

bool isSeniorDoctor(const Doctor& doctor) { return doctor.getExperience() >= 10; }
bool isMidDoctor(const Doctor& doctor) { return doctor.getExperience() >= 5; }
bool isCheapService(const Service& service) { return service.getPrice() < 40.0f; }
bool isAdultPatient(const Patient& patient) { return patient.isAdult(); }
bool isMorningAppointment(const Appointment& appointment) { return appointment.isMorning(); }
bool isNotMorningAppointment(const Appointment& appointment) { return !appointment.isMorning(); }
bool isEvenNumber(const int& number) { return number % 2 == 0; }
bool isLongWord(const string& word) { return word.size() > 4; }

bool readInt(int& value) {
    if (cin >> value) return true;
    if (cin.eof()) return false;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return false;
}

int readMenuChoice() {
    int choice;
    if (readInt(choice)) return choice;
    if (cin.eof()) return 0;
    cout << "[ERROR] Enter a number from the menu.\n";
    return -1;
}

void discardRestOfLine() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

template <typename T>
void printAll(const vector<shared_ptr<T>>& list) {
    if (list.empty()) { cout << "Empty\n"; return; }
    int count = static_cast<int>(list.size());
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << *list[i] << " [" << list[i]->classify() << "]\n";
}

void printAllFilials(const vector<shared_ptr<Filial>>& filials) {
    if (filials.empty()) { cout << "Empty\n"; return; }
    int count = static_cast<int>(filials.size());
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << filials[i]->getName() << " (" << filials[i]->getAddress() << ")\n";
}

template <typename T>
int pickIndex(const vector<shared_ptr<T>>& list) {
    printAll(list);
    if (list.empty()) return -1;
    cout << "Select index: ";
    int number;
    if (!readInt(number)) return -1;
    int index = number - 1;
    if (index < 0 || index >= static_cast<int>(list.size())) {
        cout << "[ERROR] No item with number " << number << ".\n";
        return -1;
    }
    return index;
}

template <typename T>
shared_ptr<T> pickFromList(const vector<shared_ptr<T>>& list) {
    int index = pickIndex(list);
    if (index < 0) return nullptr;
    return list[index];
}

template <typename T>
shared_ptr<T> pickFromCollection(const string& title, const Collection<T>& collection) {
    if (collection.isEmpty()) {
        cout << "[ERROR] No items to choose from: " << title << ".\n";
        return nullptr;
    }
    cout << "Select " << title << ":\n";
    collection.print(cout, "  ");
    cout << "Select index: ";
    int number;
    if (!readInt(number)) return nullptr;
    shared_ptr<T> chosen = collection.get(number - 1);
    if (!chosen) cout << "[ERROR] No item with number " << number << ".\n";
    return chosen;
}

shared_ptr<Filial> chooseFilial(const vector<shared_ptr<Filial>>& filials) {
    printAllFilials(filials);
    if (filials.empty()) return nullptr;
    cout << "Select Branch index: ";
    int number;
    if (!readInt(number)) return nullptr;
    int index = number - 1;
    if (index < 0 || index >= static_cast<int>(filials.size())) {
        cout << "[ERROR] No branch with number " << number << ".\n";
        return nullptr;
    }
    return filials[index];
}

template <typename T>
shared_ptr<T> findEqual(const vector<shared_ptr<T>>& list, const T& candidate) {
    for (const auto& item : list) {
        if (*item == candidate) return item;
    }
    return nullptr;
}

template <typename T>
void createAndStore(vector<shared_ptr<T>>& list) {
    T candidate;
    cin >> candidate;
    if (!cin) {
        discardRestOfLine();
        throw InvalidDataException("input", "a number was expected, the object was not created");
    }
    if (candidate.getName().empty()) {
        throw InvalidDataException("name", "must not be empty");
    }
    if (findEqual(list, candidate)) {
        throw DuplicateObjectException("Clinic data", candidate.getEntityType());
    }
    list.push_back(make_shared<T>(candidate));
    cout << "[OK] Object created.\n";
    logger.log("CREATE", candidate.getEntityType() + " '" + candidate.getName() + "'");
}

template <typename T>
void compareTwo(const vector<shared_ptr<T>>& list) {
    if (list.size() < 2) {
        cout << "[ERROR] At least two objects are needed.\n";
        return;
    }
    printAll(list);
    cout << "Enter two numbers: ";
    int first, second;
    if (!readInt(first) || !readInt(second)) return;
    int count = static_cast<int>(list.size());
    if (first < 1 || first > count || second < 1 || second > count) {
        cout << "[ERROR] Number is out of range.\n";
        return;
    }
    shared_ptr<T> a = list[first - 1];
    shared_ptr<T> b = list[second - 1];
    if (*a == *b) cout << "Same\n";
    else if (*a > *b) cout << a->getName() << " is >\n";
    else if (*a < *b) cout << b->getName() << " is >\n";
    else cout << "Equal\n";
}

template <typename T>
void assignToFilial(Filial& filial, const vector<shared_ptr<T>>& list) {
    shared_ptr<T> item = pickFromList(list);
    if (!item) return;
    filial += item;
    logger.log("ASSIGN", item->getEntityType() + " '" + item->getName() + "' -> branch '" + filial.getName() + "'");
}

template <typename T>
void removeFromFilial(Filial& filial, const vector<shared_ptr<T>>& list) {
    shared_ptr<T> item = pickFromList(list);
    if (!item) return;
    filial -= item;
    logger.log("REMOVE", item->getEntityType() + " '" + item->getName() + "' <- branch '" + filial.getName() + "'");
}

template <typename T>
void deleteEverywhere(const vector<shared_ptr<Filial>>& filials, vector<shared_ptr<T>>& list) {
    int index = pickIndex(list);
    if (index < 0) return;
    shared_ptr<T> victim = list[index];
    for (const auto& filial : filials) {
        if (filial->contains(victim)) *filial -= victim;
    }
    list.erase(list.begin() + index);
    cout << "[OK] Object deleted.\n";
    logger.log("DELETE", victim->getEntityType() + " '" + victim->getName() + "'");
}

void createObject(ClinicData& data) {
    cout << "1. Patient 2. Doctor 3. Service 4. Administrator\nChoice: ";
    int kind;
    if (!readInt(kind)) return;
    if (kind == 1) createAndStore(data.patients);
    else if (kind == 2) createAndStore(data.doctors);
    else if (kind == 3) createAndStore(data.services);
    else if (kind == 4) createAndStore(data.administrators);
    else cout << "[ERROR] Unknown object type.\n";
}

void assignObject(ClinicData& data) {
    shared_ptr<Filial> filial = chooseFilial(data.filials);
    if (!filial) return;
    cout << "1. Doctor 2. Service 3. Patient 4. Administrator\nChoice: ";
    int kind;
    if (!readInt(kind)) return;
    if (kind == 1) assignToFilial(*filial, data.doctors);
    else if (kind == 2) assignToFilial(*filial, data.services);
    else if (kind == 3) assignToFilial(*filial, data.patients);
    else if (kind == 4) assignToFilial(*filial, data.administrators);
    else cout << "[ERROR] Unknown object type.\n";
}

void removeObjectFromFilial(ClinicData& data) {
    shared_ptr<Filial> filial = chooseFilial(data.filials);
    if (!filial) return;
    cout << "1. Doctor 2. Service 3. Patient 4. Administrator\nChoice: ";
    int kind;
    if (!readInt(kind)) return;
    if (kind == 1) removeFromFilial(*filial, data.doctors);
    else if (kind == 2) removeFromFilial(*filial, data.services);
    else if (kind == 3) removeFromFilial(*filial, data.patients);
    else if (kind == 4) removeFromFilial(*filial, data.administrators);
    else cout << "[ERROR] Unknown object type.\n";
}

void compareObjects(ClinicData& data) {
    cout << "\n1. Compare Doctors\n";
    cout << "2. Compare Services\n";
    cout << "3. Compare Patients\n";
    cout << "4. Compare Administrators\n";
    cout << "0. Cancel\nChoice: ";
    int kind;
    if (!readInt(kind)) return;
    if (kind == 1) compareTwo(data.doctors);
    else if (kind == 2) compareTwo(data.services);
    else if (kind == 3) compareTwo(data.patients);
    else if (kind == 4) compareTwo(data.administrators);
    else if (kind != 0) cout << "[ERROR] Unknown object type.\n";
}

void deleteObject(ClinicData& data) {
    cout << "1. Doctor 2. Service 3. Patient 4. Administrator\nChoice: ";
    int kind;
    if (!readInt(kind)) return;
    if (kind == 1) deleteEverywhere(data.filials, data.doctors);
    else if (kind == 2) deleteEverywhere(data.filials, data.services);
    else if (kind == 3) deleteEverywhere(data.filials, data.patients);
    else if (kind == 4) deleteEverywhere(data.filials, data.administrators);
    else cout << "[ERROR] Unknown object type.\n";
}

void checkServiceAvailability(ClinicData& data) {
    shared_ptr<Filial> filial = chooseFilial(data.filials);
    if (!filial) return;
    shared_ptr<Service> service = pickFromList(data.services);
    if (!service) return;
    cout << (isServiceAvailable(*filial, *service) ? "Yes\n" : "No\n");
}

void findAdministrator(ClinicData& data) {
    shared_ptr<Filial> filial = chooseFilial(data.filials);
    if (!filial) return;
    cout << "Enter position: ";
    string position;
    cin >> ws;
    getline(cin, position);
    shared_ptr<Administrator> found = findAdministratorByPosition(*filial, position);
    if (found) cout << *found << "\n";
    else cout << "[INFO] No administrator with this position in the branch.\n";
}

void showPolymorphicOverview(ClinicData& data) {
    shared_ptr<Filial> filial = chooseFilial(data.filials);
    if (!filial) return;
    vector<shared_ptr<Person>> all = filial->getAllEntities();
    if (all.empty()) { cout << "Empty\n"; return; }
    for (const auto& entity : all) {
        cout << "[" << entity->getEntityType() << "] ";
        entity->printInfo(cout);
        cout << " -> " << entity->classify() << "\n";
    }
}

void bookAppointment(ClinicData& data) {
    shared_ptr<Filial> filial = chooseFilial(data.filials);
    if (!filial) return;

    shared_ptr<Patient> patient = pickFromCollection("Patient", filial->getPatients());
    if (!patient) return;
    shared_ptr<Doctor> doctor = pickFromCollection("Doctor", filial->getDoctors());
    if (!doctor) return;
    shared_ptr<Service> service = pickFromCollection("Service", filial->getServices());
    if (!service) return;

    int day, month, year, hour, minute;
    cout << "Enter date (day month year): ";
    if (!readInt(day) || !readInt(month) || !readInt(year)) return;
    if (!Appointment::isValidDate(day, month, year)) {
        throw InvalidDateTimeException("date " + to_string(day) + "." + to_string(month) + "." + to_string(year)
            + " does not exist");
    }
    cout << "Enter start time (hour minute): ";
    if (!readInt(hour) || !readInt(minute)) return;
    if (!Appointment::isValidTime(hour, minute)) {
        throw InvalidDateTimeException("time " + to_string(hour) + ":" + (minute >= 0 && minute < 10 ? "0" : "")
            + to_string(minute) + " does not exist");
    }

    shared_ptr<Appointment> appointment = make_shared<Appointment>(
        patient, doctor, service, filial->getName(), day, month, year, hour, minute);

    *filial += appointment;
    cout << "[OK] Booked: " << *appointment << "\n";
    logger.log("BOOK", "Appointment #" + to_string(appointment->getId()) + ", branch '" + filial->getName() + "'");
}

void showAppointments(ClinicData& data) {
    shared_ptr<Filial> filial = chooseFilial(data.filials);
    if (!filial) return;

    Collection<Appointment> all = filial->getAppointments();
    cout << "1. All\n2. By patient\n3. By doctor\n4. Latest\n5. Morning count\nChoice: ";
    int sub;
    if (!readInt(sub)) return;

    if (sub == 1) {
        all.print(cout, "  ");
    }
    else if (sub == 2) {
        shared_ptr<Patient> patient = pickFromCollection("Patient", filial->getPatients());
        if (!patient) return;
        Collection<Appointment> found;
        for (const auto& appointment : all) {
            if (*appointment->getPatient() == *patient) found.add(appointment);
        }
        found.print(cout, "  ");
    }
    else if (sub == 3) {
        shared_ptr<Doctor> doctor = pickFromCollection("Doctor", filial->getDoctors());
        if (!doctor) return;
        Collection<Appointment> found;
        for (const auto& appointment : all) {
            if (*appointment->getDoctor() == *doctor) found.add(appointment);
        }
        found.print(cout, "  ");
    }
    else if (sub == 4) {
        shared_ptr<Appointment> latest = findMax(all);
        if (latest) cout << "-> Latest: " << *latest << "\n";
        else cout << "[INFO] No appointments in this branch.\n";
    }
    else if (sub == 5) {
        cout << "-> Morning: " << countIf(all, isMorningAppointment)
            << ", later in the day: " << countIf(all, isNotMorningAppointment) << "\n";
    }
    else {
        cout << "[ERROR] Unknown option.\n";
    }
}

void cancelAppointment(ClinicData& data) {
    shared_ptr<Filial> filial = chooseFilial(data.filials);
    if (!filial) return;

    shared_ptr<Appointment> appointment = pickFromCollection("Appointment", filial->getAppointments());
    if (!appointment) return;

    *filial -= appointment;
    cout << "[OK] Appointment cancelled.\n";
    logger.log("CANCEL", "Appointment #" + to_string(appointment->getId()) + ", branch '" + filial->getName() + "'");
}

void demoAddAndSize() {
    cout << "\n--- 1. Add & Size Methods ---\n";
    Collection<Doctor> demoDocs;

    cout << "Calling isEmpty()... -> " << (demoDocs.isEmpty() ? "True" : "False") << "\n";
    cout << "Calling add()... Adding 2 temporary doctors.\n";
    demoDocs.add(make_shared<Doctor>("Andrei Ivanov", "Therapist", 5));
    demoDocs.add(make_shared<Doctor>("Sergei Krug", "Surgeon", 10));
    cout << "Calling add(nullptr). -> ";
    try {
        demoDocs.add(nullptr);
        cout << "added\n";
    }
    catch (const NullObjectException& error) {
        cout << "Exception safely caught! (" << error.what() << ")\n";
    }

    cout << "Calling isEmpty()... -> " << (demoDocs.isEmpty() ? "True" : "False") << "\n";
    cout << "Calling getCount().. -> " << demoDocs.getCount() << "\n";
    cout << "Calling print()..... ->\n";
    demoDocs.print(cout, "  ");
}

void demoAccessAndSearch(const Filial& filial) {
    cout << "\n--- 2. Access & Search Methods ---\n";
    Collection<Doctor> docs = filial.getDoctors();
    if (docs.isEmpty()) {
        cout << "[ERROR] Need doctors in branch to test search!\n";
        return;
    }

    shared_ptr<Doctor> target = docs.get(0);
    cout << "Calling get(0)........ -> " << target->getName() << "\n";
    cout << "Calling get(100)...... -> " << (docs.get(100) ? "found" : "nullptr") << "\n";

    cout << "Calling at(100)....... -> ";
    try {
        docs.at(100);
        cout << "found\n";
    }
    catch (const IndexOutOfRangeException& error) {
        cout << "Exception safely caught! (" << error.what() << ")\n";
    }

    cout << "Calling indexOf()..... -> " << docs.indexOf(*target) << "\n";
    cout << "Calling contains().... -> " << (docs.contains(*target) ? "True" : "False") << "\n";

    shared_ptr<Doctor> found = docs.find(*target);
    cout << "Calling find()........ -> " << (found ? "Object successfully found in memory" : "Not found") << "\n";
}

void demoRemoval(const Filial& filial) {
    cout << "\n--- 3. Removal Methods (Working on a COPY of the list) ---\n";
    Collection<Doctor> demoDocs = filial.getDoctors();
    if (demoDocs.isEmpty()) {
        cout << "[ERROR] Need doctors in branch to test removal!\n";
        return;
    }

    cout << "Initial count: " << demoDocs.getCount() << "\n";

    demoDocs.removeAt(0);
    cout << "After removeAt(0).... count is: " << demoDocs.getCount() << "\n";

    if (!demoDocs.isEmpty()) {
        shared_ptr<Doctor> target = demoDocs.get(0);
        demoDocs.remove(*target);
        cout << "After remove(value).. count is: " << demoDocs.getCount() << "\n";
    }

    int removedByCondition = demoDocs.removeIf(isSeniorDoctor);
    cout << "After removeIf(Senior) removed " << removedByCondition
        << " doctors. Remaining: " << demoDocs.getCount() << "\n";

    demoDocs.clear();
    cout << "After clear()........ isEmpty() is -> " << (demoDocs.isEmpty() ? "True" : "False") << "\n";
    cout << "The branch itself still has " << filial.getDoctorCount() << " doctors.\n";
}

void demoAlgorithms(const Filial& filial) {
    cout << "\n--- 4. Template Algorithms (one set of functions, three user types) ---\n";

    Collection<Doctor> docs = filial.getDoctors();
    shared_ptr<Doctor> senior = findFirst(docs, isSeniorDoctor);
    cout << "Doctors:\n";
    cout << "  findFirst(isSeniorDoctor) ... -> " << (senior ? senior->getName() : "None") << "\n";
    cout << "  findAll(isMidDoctor)......... -> Found " << findAll(docs, isMidDoctor).getCount() << " doctors\n";
    cout << "  countIf(isSeniorDoctor)...... -> " << countIf(docs, isSeniorDoctor) << "\n";
    shared_ptr<Doctor> mostExperienced = findMax(docs);
    cout << "  findMax() [Highest exp]...... -> " << (mostExperienced ? mostExperienced->getName() : "None") << "\n";

    Collection<Service> services = filial.getServices();
    shared_ptr<Service> cheap = findFirst(services, isCheapService);
    cout << "Services:\n";
    cout << "  findFirst(isCheapService).... -> " << (cheap ? cheap->getName() : "None") << "\n";
    shared_ptr<Service> mostExpensive = findMax(services);
    cout << "  findMax() [Highest price].... -> " << (mostExpensive ? mostExpensive->getName() : "None") << "\n";

    Collection<Patient> patients = filial.getPatients();
    shared_ptr<Patient> oldest = findMax(patients);
    cout << "Patients:\n";
    cout << "  countIf(isAdultPatient)...... -> " << countIf(patients, isAdultPatient) << "\n";
    cout << "  findMax() [Oldest]........... -> " << (oldest ? oldest->getName() : "None") << "\n";

    Collection<Appointment> appointments = filial.getAppointments();
    cout << "Appointments:\n";
    cout << "  countIf(isMorningAppointment) -> " << countIf(appointments, isMorningAppointment) << "\n";
    shared_ptr<Appointment> latest = findMax(appointments);
    if (latest) cout << "  findMax() [Latest]........... -> " << *latest << "\n";
    else cout << "  findMax() [Latest]........... -> None (book an appointment first)\n";
}

void demoDomainIndependence() {
    cout << "\n--- 5. Domain Independence (Primitive Types) ---\n";

    Collection<int> numbers;
    for (int i = 1; i <= 5; i++) numbers.add(make_shared<int>(i * 10));
    Collection<string> words;
    words.add(make_shared<string>("apple"));
    words.add(make_shared<string>("banana"));
    words.add(make_shared<string>("fig"));

    cout << "Collection<int> numbers:\n";
    numbers.print(cout, "  ");
    cout << "findMax() in numbers ........ -> " << *findMax(numbers) << "\n";
    cout << "countIf(isEvenNumber) ....... -> " << countIf(numbers, isEvenNumber) << "\n";

    cout << "\nCollection<string> words:\n";
    words.print(cout, "  ");
    shared_ptr<string> longWord = findFirst(words, isLongWord);
    if (longWord) cout << "findFirst(isLongWord) ....... -> " << *longWord << "\n";
    else cout << "findFirst(isLongWord) ....... -> None\n";
}

void runCollectionDemo(ClinicData& data) {
    shared_ptr<Filial> filial = chooseFilial(data.filials);
    if (!filial) return;

    int choice = -1;
    while (choice != 0) {
        cout << "\n=== COLLECTION<T> FULL DEMO MENU (" << filial->getName() << ") ===\n";
        cout << "1. Add & Size methods (add, getCount, isEmpty, print)\n";
        cout << "2. Access & Search methods (get, at, indexOf, contains, find)\n";
        cout << "3. Removal methods (removeAt, remove, removeIf, clear)\n";
        cout << "4. Template Algorithms (findFirst, findAll, countIf, findMax)\n";
        cout << "5. Domain Independence (Collection<int>, Collection<string>)\n";
        cout << "0. Go back\nChoice: ";
        choice = readMenuChoice();

        if (choice == 1) demoAddAndSize();
        else if (choice == 2) demoAccessAndSearch(*filial);
        else if (choice == 3) demoRemoval(*filial);
        else if (choice == 4) demoAlgorithms(*filial);
        else if (choice == 5) demoDomainIndependence();
        else if (choice != 0 && choice != -1) cout << "[ERROR] Unknown option.\n";
    }
}

struct DemoWorld {
    shared_ptr<Doctor> doctor;
    shared_ptr<Patient> patient;
    shared_ptr<Service> service;
    Filial filial;

    DemoWorld()
        : doctor(make_shared<Doctor>("Demo Doctor", "Therapist", 7)),
        patient(make_shared<Patient>("Demo Patient", 30, "+375290000999")),
        service(make_shared<Service>("Demo Checkup", 30.0f, 30)),
        filial("Demo Branch", "Test st 1", 1, 1, 1, 1, 3) {
        filial += doctor;
        filial += service;
        filial += patient;
    }

    shared_ptr<Appointment> makeAppointment(int day, int month, int year, int hour, int minute) const {
        return make_shared<Appointment>(patient, doctor, service, filial.getName(), day, month, year, hour, minute);
    }
};

class TraceGuard {
private:
    string label;

public:
    explicit TraceGuard(const string& text) : label(text) {
        cout << "  [enter] " << label << "\n";
    }
    ~TraceGuard() noexcept {
        printf("  [leave] %s (destructor called)\n", label.c_str());
    }
    TraceGuard(const TraceGuard&) = delete;
    TraceGuard& operator=(const TraceGuard&) = delete;
};

string describeDemoWorld() {
    return "Demo Branch (limits: doctors 1, services 1, patients 1, administrators 1, appointments 3); "
        "registered: Doctor 'Demo Doctor', Service 'Demo Checkup' (30 min), Patient 'Demo Patient'";
}

void printScenarioHeader(int number, const string& exceptionName, const string& situation) {
    cout << "\n==================================================\n";
    cout << number << ". " << exceptionName << "\n";
    cout << "Situation:  " << situation << "\n";
}

void printCaught(const string& typeName, const exception& error) {
    cout << "Result:     exception " << typeName << " was thrown and caught\n";
    cout << "Message:    " << error.what() << "\n";
}

void printContinues() {
    cout << "The program continues to work.\n";
}

void printNoError() {
    cout << "Result:     no exception (unexpected)\n";
}

void scenarioInvalidData() {
    printScenarioHeader(1, "InvalidDataException", "a setter receives an incorrect value");
    Doctor doctor("Test Doctor", "Therapist", 3);
    cout << "Test data:  Doctor 'Test Doctor', experience 3; new experience = -5\n";
    cout << "Action:     doctor.setExperience(-5)\n";
    try {
        doctor.setExperience(-5);
        printNoError();
    }
    catch (const InvalidDataException& error) {
        printCaught("InvalidDataException (field '" + error.getField() + "')", error);
        cout << "Object:     experience is still " << doctor.getExperience() << " (unchanged)\n";
        printContinues();
    }
}

void scenarioConstructor() {
    printScenarioHeader(2, "InvalidDataException (constructor)", "an object is created with incorrect data");
    cout << "Test data:  Filial('Broken Branch', 'Nowhere', doctorCapacity = -1)\n";
    cout << "Action:     creating the branch object\n";
    try {
        Filial broken("Broken Branch", "Nowhere", -1);
        printNoError();
    }
    catch (const InvalidDataException& error) {
        printCaught("InvalidDataException (field '" + error.getField() + "')", error);
        cout << "Object:     the branch was never created\n";
        printContinues();
    }
}

void scenarioInvalidDateTime() {
    printScenarioHeader(3, "InvalidDateTimeException", "an appointment is booked on a date that does not exist");
    DemoWorld world;
    cout << "Branch:     " << describeDemoWorld() << "\n";
    cout << "Test data:  patient 'Demo Patient', doctor 'Demo Doctor', service 'Demo Checkup', date 31.02.2026, time 10:00\n";
    cout << "Action:     filial += appointment\n";
    try {
        world.filial += world.makeAppointment(31, 2, 2026, 10, 0);
        printNoError();
    }
    catch (const InvalidDateTimeException& error) {
        printCaught("InvalidDateTimeException", error);
        cout << "Branch:     appointments " << world.filial.getAppointmentCount() << " (unchanged)\n";
        printContinues();
    }
}

void scenarioNullObject() {
    printScenarioHeader(4, "NullObjectException", "an empty pointer is passed instead of an object");
    DemoWorld world;
    shared_ptr<Doctor> nobody;
    cout << "Branch:     " << describeDemoWorld() << "\n";
    cout << "Test data:  an empty shared_ptr<Doctor> (points to nothing)\n";
    cout << "Action:     filial += nobody\n";
    try {
        world.filial += nobody;
        printNoError();
    }
    catch (const NullObjectException& error) {
        printCaught("NullObjectException (" + error.getObjectType() + ")", error);
        cout << "Branch:     doctors " << world.filial.getDoctorCount() << " (unchanged)\n";
        printContinues();
    }
}

void scenarioDuplicate() {
    printScenarioHeader(5, "DuplicateObjectException", "an object that is already in the branch is added again");
    DemoWorld world;
    cout << "Branch:     " << describeDemoWorld() << "\n";
    cout << "Test data:  Doctor 'Demo Doctor' (already registered)\n";
    cout << "Action:     filial += doctor\n";
    try {
        world.filial += world.doctor;
        printNoError();
    }
    catch (const DuplicateObjectException& error) {
        printCaught("DuplicateObjectException (" + error.getObjectType() + ")", error);
        cout << "Branch:     doctors " << world.filial.getDoctorCount() << " (unchanged)\n";
        printContinues();
    }
}

void scenarioNotFound() {
    printScenarioHeader(6, "ObjectNotFoundException", "an object that is not in the branch is removed");
    DemoWorld world;
    cout << "Branch:     " << describeDemoWorld() << "\n";
    cout << "Test data:  Doctor 'Stranger Doctor' (never added to the branch)\n";
    cout << "Action:     filial -= doctor\n";
    try {
        world.filial -= make_shared<Doctor>("Stranger Doctor", "Surgeon", 3);
        printNoError();
    }
    catch (const ObjectNotFoundException& error) {
        printCaught("ObjectNotFoundException (" + error.getObjectType() + ")", error);
        cout << "Branch:     doctors " << world.filial.getDoctorCount() << " (unchanged)\n";
        printContinues();
    }
}

void scenarioCapacity() {
    printScenarioHeader(7, "CapacityExceededException", "the limit of objects in the branch is exceeded");
    DemoWorld world;
    cout << "Branch:     " << describeDemoWorld() << "\n";
    cout << "Test data:  second doctor 'Second Doctor' (the branch holds only 1 doctor)\n";
    cout << "Before:     doctors " << world.filial.getDoctorCount() << "\n";
    cout << "Action:     filial += doctor\n";
    try {
        world.filial += make_shared<Doctor>("Second Doctor", "Surgeon", 4);
        printNoError();
    }
    catch (const CapacityExceededException& error) {
        printCaught("CapacityExceededException (limit " + to_string(error.getCapacity()) + ")", error);
        cout << "After:      doctors " << world.filial.getDoctorCount() << " (unchanged)\n";
        printContinues();
    }
}

void scenarioLinkViolation() {
    printScenarioHeader(8, "LinkViolationException", "an appointment is linked to an object that is not registered in the branch");
    DemoWorld world;
    shared_ptr<Patient> stranger = make_shared<Patient>("Stranger Patient", 40, "+375290000777");
    shared_ptr<Appointment> appointment = make_shared<Appointment>(
        stranger, world.doctor, world.service, world.filial.getName(), 15, 3, 2026, 10, 0);
    cout << "Branch:     " << describeDemoWorld() << "\n";
    cout << "Test data:  Patient 'Stranger Patient' (not registered), Doctor 'Demo Doctor', Service 'Demo Checkup', 15.03.2026 10:00\n";
    cout << "Action:     filial += appointment\n";
    try {
        world.filial += appointment;
        printNoError();
    }
    catch (const LinkViolationException& error) {
        printCaught("LinkViolationException", error);
        cout << "Branch:     appointments " << world.filial.getAppointmentCount() << " (unchanged)\n";
        printContinues();
    }
}

void scenarioWorkingHours() {
    printScenarioHeader(9, "WorkingHoursException", "an appointment starts before the clinic opens");
    DemoWorld world;
    cout << "Branch:     " << describeDemoWorld() << "\n";
    cout << "Test data:  15.03.2026, start 07:30, service length 30 min, clinic works 08:00-20:00\n";
    cout << "Action:     filial += appointment\n";
    try {
        world.filial += world.makeAppointment(15, 3, 2026, 7, 30);
        printNoError();
    }
    catch (const WorkingHoursException& error) {
        printCaught("WorkingHoursException (working hours " + error.getWorkingHours() + ")", error);
        cout << "Branch:     appointments " << world.filial.getAppointmentCount() << " (unchanged)\n";
        printContinues();
    }
}

void scenarioScheduleConflict() {
    printScenarioHeader(10, "ScheduleConflictException", "the doctor is already busy at the requested time");
    DemoWorld world;
    cout << "Branch:     " << describeDemoWorld() << "\n";
    cout << "Test data:  first appointment 15.03.2026 10:00-10:30, second appointment 15.03.2026 10:15 (same doctor)\n";
    cout << "Action 1:   filial += appointment at 10:00\n";
    world.filial += world.makeAppointment(15, 3, 2026, 10, 0);
    cout << "            booked, appointments: " << world.filial.getAppointmentCount() << "\n";
    cout << "Action 2:   filial += appointment at 10:15\n";
    try {
        world.filial += world.makeAppointment(15, 3, 2026, 10, 15);
        printNoError();
    }
    catch (const ScheduleConflictException& error) {
        printCaught("ScheduleConflictException (" + error.getPersonType() + " '" + error.getPersonName() + "')", error);
        cout << "Branch:     appointments " << world.filial.getAppointmentCount() << " (unchanged)\n";
        printContinues();
    }
}

void scenarioInvalidOperation() {
    printScenarioHeader(11, "InvalidOperationException", "an operation is not allowed in the current state of the branch");
    DemoWorld world;
    Filial empty("Empty Branch", "Nowhere", 1, 1, 1, 1, 1);
    cout << "Branch:     Empty Branch (no doctors at all)\n";
    cout << "Test data:  Patient 'Demo Patient'\n";
    cout << "Action:     emptyBranch += patient\n";
    try {
        empty += world.patient;
        printNoError();
    }
    catch (const InvalidOperationException& error) {
        printCaught("InvalidOperationException (operation '" + error.getOperation() + "')", error);
        cout << "Branch:     patients " << empty.getPatientCount() << " (unchanged)\n";
        printContinues();
    }
}

void scenarioIndexOutOfRange() {
    printScenarioHeader(12, "IndexOutOfRangeException", "an element is requested outside the allowed range");
    DemoWorld world;
    Collection<Doctor> doctors = world.filial.getDoctors();
    cout << "Collection: list of doctors of the Demo Branch, size " << doctors.getCount() << "\n";
    cout << "Test data:  index 5\n";
    cout << "Action:     doctors.at(5)\n";
    try {
        doctors.at(5);
        printNoError();
    }
    catch (const IndexOutOfRangeException& error) {
        printCaught("IndexOutOfRangeException (index " + to_string(error.getIndex())
            + ", size " + to_string(error.getSize()) + ")", error);
        printContinues();
    }
}

void demoOwnExceptions() {
    cout << "\n--- ALL own exceptions, one after another ---\n";
    scenarioInvalidData();
    scenarioConstructor();
    scenarioInvalidDateTime();
    scenarioNullObject();
    scenarioDuplicate();
    scenarioNotFound();
    scenarioCapacity();
    scenarioLinkViolation();
    scenarioWorkingHours();
    scenarioScheduleConflict();
    scenarioInvalidOperation();
    scenarioIndexOutOfRange();
    cout << "\n--- All 12 situations were handled, the program did not stop ---\n";
}

void throwByNumber(int number) {
    if (number == 1) throw InvalidDateTimeException("'31.02.2026 10:00' is not a valid date and time");
    if (number == 2) throw InvalidDataException("age", "must not be negative, got -1");
    if (number == 3) throw ObjectNotFoundException("Branch 'Demo'", "Doctor");
    if (number == 4) throw IndexOutOfRangeException(7, 3);
    if (number == 5) throw runtime_error("an error from the standard library");
    if (number == 6) throw 42;
}

void demoCatchOrder() {
    cout << "\n--- One try block, several catch blocks (derived classes first, catch(...) last) ---\n";
    for (int number = 1; number <= 6; number++) {
        cout << number << ". ";
        try {
            throwByNumber(number);
        }
        catch (const InvalidDateTimeException& error) {
            cout << "handler InvalidDateTimeException: " << error.what() << "\n";
        }
        catch (const InvalidDataException& error) {
            cout << "handler InvalidDataException (base of InvalidDateTimeException): " << error.what() << "\n";
        }
        catch (const ClinicException& error) {
            cout << "handler ClinicException (common base of all own exceptions): " << error.what() << "\n";
        }
        catch (const exception& error) {
            cout << "handler std::exception: " << error.what() << "\n";
        }
        catch (...) {
            cout << "handler catch(...): an exception of an unknown type\n";
        }
    }
}

void unwindLevelThree() {
    TraceGuard guard("level 3");
    Collection<Doctor> doctors;
    cout << "  level 3 asks for doctor number 0 of an empty collection\n";
    doctors.at(0);
    cout << "  this line is never reached\n";
}

void unwindLevelTwo() {
    TraceGuard guard("level 2");
    unwindLevelThree();
    cout << "  this line is never reached\n";
}

void unwindLevelOne() {
    TraceGuard guard("level 1");
    unwindLevelTwo();
    cout << "  this line is never reached\n";
}

void demoStackUnwinding() {
    cout << "\n--- Stack unwinding: local objects are destroyed while the exception travels up ---\n";
    try {
        unwindLevelOne();
    }
    catch (const IndexOutOfRangeException& error) {
        cout << "  Caught at the top: " << error.what() << "\n";
    }
    cout << "  The program continues normally.\n";
}

void demoRethrowAndState() {
    cout << "\n--- Rethrow with 'throw;' and the state of the object after an error ---\n";
    DemoWorld world;
    cout << "Patients before: " << world.filial.getPatientCount() << "\n";
    try {
        try {
            world.filial += world.patient;
        }
        catch (const DuplicateObjectException& error) {
            cout << "Inner handler logs: " << error.what() << "\n";
            cout << "Inner handler cannot fix it and passes it up with 'throw;'\n";
            throw;
        }
    }
    catch (const ClinicException& error) {
        cout << "Outer handler received the same exception: " << error.what() << "\n";
    }
    cout << "Patients after: " << world.filial.getPatientCount() << " (the failed operation changed nothing)\n";
    cout << world.filial;
}

void printExceptionsDemoMenu() {
    cout << "\n=== EXCEPTIONS DEMO MENU ===\n";
    cout << " 1. InvalidDataException         - setter gets a negative value\n";
    cout << " 2. InvalidDataException         - constructor gets incorrect data\n";
    cout << " 3. InvalidDateTimeException     - appointment on 31.02.2026\n";
    cout << " 4. NullObjectException          - empty pointer instead of an object\n";
    cout << " 5. DuplicateObjectException     - the same doctor added twice\n";
    cout << " 6. ObjectNotFoundException      - removing a doctor that is not in the branch\n";
    cout << " 7. CapacityExceededException    - second doctor in a branch with limit 1\n";
    cout << " 8. LinkViolationException       - appointment for an unregistered patient\n";
    cout << " 9. WorkingHoursException        - appointment at 07:30\n";
    cout << "10. ScheduleConflictException    - doctor is busy at 10:15\n";
    cout << "11. InvalidOperationException    - patient added to a branch without doctors\n";
    cout << "12. IndexOutOfRangeException     - Collection::at(5)\n";
    cout << "13. RUN ALL 12 situations\n";
    cout << "14. Several catch blocks and catch(...), order of handlers\n";
    cout << "15. Stack unwinding (destructors of local objects)\n";
    cout << "16. Rethrow with throw; and object state after an error\n";
    cout << " 0. Go back\nChoice: ";
}

void runExceptionsDemo() {
    int choice = -1;
    while (choice != 0) {
        printExceptionsDemoMenu();
        choice = readMenuChoice();

        if (choice == 1) scenarioInvalidData();
        else if (choice == 2) scenarioConstructor();
        else if (choice == 3) scenarioInvalidDateTime();
        else if (choice == 4) scenarioNullObject();
        else if (choice == 5) scenarioDuplicate();
        else if (choice == 6) scenarioNotFound();
        else if (choice == 7) scenarioCapacity();
        else if (choice == 8) scenarioLinkViolation();
        else if (choice == 9) scenarioWorkingHours();
        else if (choice == 10) scenarioScheduleConflict();
        else if (choice == 11) scenarioInvalidOperation();
        else if (choice == 12) scenarioIndexOutOfRange();
        else if (choice == 13) demoOwnExceptions();
        else if (choice == 14) demoCatchOrder();
        else if (choice == 15) demoStackUnwinding();
        else if (choice == 16) demoRethrowAndState();
        else if (choice != 0 && choice != -1) cout << "[ERROR] Unknown option.\n";
    }
}

void fillDemoData(ClinicData& data) {
    data.services.push_back(make_shared<Service>("Therapy Session", 25.0f, 15));
    data.services.push_back(make_shared<Service>("Massage", 50.0f, 30));
    data.services.push_back(make_shared<Service>("MRI Scan", 150.0f, 45));
    data.services.push_back(make_shared<Service>("Blood Test", 10.0f, 5));
    data.services.push_back(make_shared<Service>("Dental Surgery", 200.0f, 60));

    data.doctors.push_back(make_shared<Doctor>("Ivanov Ivan", "Therapist", 5));
    data.doctors.push_back(make_shared<Doctor>("House Gregory", "Diagnostician", 20));
    data.doctors.push_back(make_shared<Doctor>("Watson John", "Surgeon", 8));
    data.doctors.push_back(make_shared<Doctor>("Dorian John", "Intern", 1));
    data.doctors.push_back(make_shared<Doctor>("Quinn Medicine", "Pediatrician", 12));

    data.patients.push_back(make_shared<Patient>("Smirnov Oleg", 34, "+375291112233"));
    data.patients.push_back(make_shared<Patient>("Potter Harry", 15, "+375290000001"));
    data.patients.push_back(make_shared<Patient>("Wayne Bruce", 45, "+375290000002"));
    data.patients.push_back(make_shared<Patient>("Granger Hermione", 17, "+375290000003"));
    data.patients.push_back(make_shared<Patient>("Stark Tony", 50, "+375290000004"));

    data.administrators.push_back(make_shared<Administrator>("Petrov Petr", "Head Physician", 25, 50));
    data.administrators.push_back(make_shared<Administrator>("Scott Michael", "Regional Manager", 15, 30));
    data.administrators.push_back(make_shared<Administrator>("Swanson Ron", "Director", 5, 10));
    data.administrators.push_back(make_shared<Administrator>("Moss Maurice", "IT Support", 2, 5));

    data.filials.push_back(make_shared<Filial>("Central Clinic", "Main st 10"));
    data.filials.push_back(make_shared<Filial>("North Branch", "Cold st 1"));
    data.filials.push_back(make_shared<Filial>("South Branch", "Sunny blvd 99"));

    Filial& central = *data.filials[0];
    central += data.doctors[0];
    central += data.doctors[1];
    central += data.services[0];
    central += data.services[2];
    central += data.patients[0];
    central += data.patients[2];
    central += data.administrators[0];

    Filial& north = *data.filials[1];
    north += data.doctors[3];
    north += data.services[3];
    north += data.patients[1];
    north += data.administrators[1];
    north += data.administrators[3];

    Filial& south = *data.filials[2];
    south += data.doctors[2];
    south += data.doctors[4];
    south += data.services[1];
    south += data.services[4];
    south += data.patients[3];
    south += data.patients[4];
    south += data.administrators[2];
}

void saveState(const ClinicData& data) {
    DataStorage storage(STATE_FILE_NAME);
    storage.save(data);
    cout << "[OK] State saved to '" << STATE_FILE_NAME << "'.\n";
    logger.log("SAVE", string("State saved to ") + STATE_FILE_NAME);
}

void loadState(ClinicData& data) {
    DataStorage storage(STATE_FILE_NAME);
    data = storage.load();
    cout << "[OK] State loaded from '" << STATE_FILE_NAME << "'.\n";
    logger.log("LOAD", string("State loaded from ") + STATE_FILE_NAME);
}

void makeReport(const ClinicData& data) {
    ReportWriter writer(REPORT_FILE_NAME);
    writer.write(data);
    cout << "[OK] Report saved to '" << REPORT_FILE_NAME << "'.\n";
    logger.log("REPORT", string("Report created in ") + REPORT_FILE_NAME);
}

void printMainMenu() {
    cout << "\n1. View Branches\n2. View Doctors\n3. View Services\n4. View Patients\n5. View Admins\n"
        << "6. Create Object\n7. Assign Object\n8. Remove Object\n9. Compare\n10. Delete Object\n"
        << "11. Check Service\n12. Find Admin\n13. Polymorphic Overview\n14. Book Appt\n15. View Appts\n"
        << "16. Cancel Appt\n17. Collection Demo\n18. Exceptions Demo\n"
        << "19. Save State\n20. Load State\n21. Make Report\n0. Exit\nChoice: ";
}

void executeMenuChoice(ClinicData& data, int choice) {
    switch (choice) {
    case 1:
        for (const auto& filial : data.filials) cout << *filial;
        break;
    case 2: printAll(data.doctors); break;
    case 3: printAll(data.services); break;
    case 4: printAll(data.patients); break;
    case 5: printAll(data.administrators); break;
    case 6: createObject(data); break;
    case 7: assignObject(data); break;
    case 8: removeObjectFromFilial(data); break;
    case 9: compareObjects(data); break;
    case 10: deleteObject(data); break;
    case 11: checkServiceAvailability(data); break;
    case 12: findAdministrator(data); break;
    case 13: showPolymorphicOverview(data); break;
    case 14: bookAppointment(data); break;
    case 15: showAppointments(data); break;
    case 16: cancelAppointment(data); break;
    case 17: runCollectionDemo(data); break;
    case 18: runExceptionsDemo(); break;
    case 19: saveState(data); break;
    case 20: loadState(data); break;
    case 21: makeReport(data); break;
    case 0:
    case -1:
        break;
    default:
        cout << "[ERROR] Unknown menu item.\n";
        break;
    }
}

void reportProblem(const string& tag, const string& message, const string& hint) {
    cout << "[" << tag << "] " << message << "\n";
    if (!hint.empty()) cout << hint << "\n";
    logger.log("ERROR", "[" + tag + "] " + message);
}

void runMenuSafely(ClinicData& data, int choice) {
    try {
        executeMenuChoice(data, choice);
    }
    catch (const InvalidDateTimeException& error) {
        reportProblem("INVALID DATE/TIME", error.what(),
            "Hint: enter the date as 'day month year' and the time as 'hour minute'.");
    }
    catch (const InvalidDataException& error) {
        reportProblem("INVALID DATA", error.what(),
            "Nothing was changed. Check the field '" + error.getField() + "' and try again.");
    }
    catch (const NullObjectException& error) {
        reportProblem("EMPTY OBJECT", error.what(), "The operation was skipped.");
    }
    catch (const DuplicateObjectException& error) {
        reportProblem("DUPLICATE", error.what(), "Choose another " + error.getObjectType() + ".");
    }
    catch (const ObjectNotFoundException& error) {
        reportProblem("NOT FOUND", error.what(), "Look at the content of the branch with menu item 1.");
    }
    catch (const CapacityExceededException& error) {
        reportProblem("CAPACITY", error.what(),
            "Remove a " + error.getObjectType() + " from the branch (menu item 8) or use another branch.");
    }
    catch (const LinkViolationException& error) {
        reportProblem("BROKEN LINK", error.what(), "Assign the missing object to the branch first (menu item 7).");
    }
    catch (const WorkingHoursException& error) {
        reportProblem("WORKING HOURS", error.what(), "Choose an earlier start or a shorter service.");
    }
    catch (const ScheduleConflictException& error) {
        reportProblem("SCHEDULE CONFLICT", error.what(), "Choose another time or another " + error.getPersonType() + ".");
    }
    catch (const InvalidOperationException& error) {
        reportProblem("NOT ALLOWED", error.what(), "");
    }
    catch (const IndexOutOfRangeException& error) {
        reportProblem("INDEX", error.what(), "");
    }
    catch (const FileOpenException& error) {
        reportProblem("FILE", error.what(), "The state of the program was not changed.");
    }
    catch (const FileIOException& error) {
        reportProblem("FILE I/O", error.what(), "The state of the program was not changed.");
    }
    catch (const FileFormatException& error) {
        reportProblem("DAMAGED FILE", error.what(), "The file was not loaded. The state of the program was not changed.");
    }
    catch (const ClinicException& error) {
        reportProblem("CLINIC ERROR", error.what(), "");
    }
    catch (const exception& error) {
        reportProblem("SYSTEM ERROR", error.what(), "");
    }
    catch (...) {
        cout << "[UNKNOWN ERROR] An unexpected error was caught, the program continues.\n";
        logger.log("ERROR", "An exception of an unknown type");
    }
}

bool initializeState(ClinicData& data) {
    error_code error;
    if (!filesystem::exists(STATE_FILE_NAME, error)) {
        try {
            fillDemoData(data);
            logger.log("SEED", "No saved state, demo data created");
        }
        catch (const ClinicException& problem) {
            reportProblem("ERROR", string("Start data was not loaded completely: ") + problem.what(), "");
        }
        return true;
    }

    try {
        DataStorage storage(STATE_FILE_NAME);
        data = storage.load();
        cout << "[OK] State restored from '" << STATE_FILE_NAME << "'.\n";
        logger.log("AUTOLOAD", string("State loaded from ") + STATE_FILE_NAME + " at start");
        return true;
    }
    catch (const ClinicException& problem) {
        reportProblem("DAMAGED FILE", problem.what(), "The program starts with an empty state.");
        cout << "Auto-save at exit is switched off, so that the file '" << STATE_FILE_NAME
            << "' is not overwritten. Use menu item 19 to save manually.\n";
        logger.log("AUTOSAVE_DISABLED", "The state file could not be loaded");
        return false;
    }
}

void autoSaveState(const ClinicData& data) {
    try {
        DataStorage storage(STATE_FILE_NAME);
        storage.save(data);
        cout << "[OK] State saved to '" << STATE_FILE_NAME << "' at exit.\n";
        logger.log("AUTOSAVE", string("State saved to ") + STATE_FILE_NAME + " at exit");
    }
    catch (const ClinicException& problem) {
        reportProblem("FILE", problem.what(), "The state was not saved at exit.");
    }
}

int main() {
    ClinicData data;
    logger.log("START", "Program started");
    bool autoSaveEnabled = initializeState(data);

    int mainChoice = -1;
    while (mainChoice != 0) {
        printMainMenu();
        mainChoice = readMenuChoice();
        runMenuSafely(data, mainChoice);
    }

    if (autoSaveEnabled) autoSaveState(data);
    logger.log("EXIT", "Program finished");
    return 0;
}
