#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <stdexcept>
#include "service.h"
#include "doctor.h"
#include "patient.h"
#include "administrator.h"
#include "filial.h"
#include "person.h"
#include "collection.h"
#include "appointment.h"

using namespace std;

struct ClinicData {
    vector<shared_ptr<Service>> services;
    vector<shared_ptr<Doctor>> doctors;
    vector<shared_ptr<Patient>> patients;
    vector<shared_ptr<Administrator>> administrators;
    vector<shared_ptr<Filial>> filials;
};

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
        cout << "[ERROR] Invalid input, object was not created.\n";
        return;
    }
    if (candidate.getName().empty()) {
        cout << "[ERROR] Name must not be empty.\n";
        return;
    }
    if (findEqual(list, candidate)) {
        cout << "[ERROR] Such object already exists.\n";
        return;
    }
    list.push_back(make_shared<T>(candidate));
    cout << "[OK] Object created.\n";
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
    if (item) filial += item;
}

template <typename T>
void removeFromFilial(Filial& filial, const vector<shared_ptr<T>>& list) {
    shared_ptr<T> item = pickFromList(list);
    if (item) filial -= item;
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
        cout << "[ERROR] Invalid date.\n";
        return;
    }
    cout << "Enter start time (hour minute): ";
    if (!readInt(hour) || !readInt(minute)) return;
    if (!Appointment::isValidTime(hour, minute)) {
        cout << "[ERROR] Invalid time.\n";
        return;
    }

    shared_ptr<Appointment> appointment = make_shared<Appointment>(
        patient, doctor, service, filial->getName(), day, month, year, hour, minute);

    int countBefore = filial->getAppointmentCount();
    *filial += appointment;
    if (filial->getAppointmentCount() > countBefore) {
        cout << "[OK] Booked: " << *appointment << "\n";
    }
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

    int countBefore = filial->getAppointmentCount();
    *filial -= appointment;
    if (filial->getAppointmentCount() < countBefore) {
        cout << "[OK] Appointment cancelled.\n";
    }
}

void demoAddAndSize() {
    cout << "\n--- 1. Add & Size Methods ---\n";
    Collection<Doctor> demoDocs;

    cout << "Calling isEmpty()... -> " << (demoDocs.isEmpty() ? "True" : "False") << "\n";
    cout << "Calling add()... Adding 2 temporary doctors.\n";
    demoDocs.add(make_shared<Doctor>("Andrei Ivanov", "Therapist", 5));
    demoDocs.add(make_shared<Doctor>("Sergei Krug", "Surgeon", 10));
    cout << "Calling add(nullptr). -> " << (demoDocs.add(nullptr) ? "added" : "rejected") << "\n";

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
    catch (const out_of_range& error) {
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
    words.add(make_shared<string>("passionfruit"));

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

void printMainMenu() {
    cout << "\n1. View Branches\n2. View Doctors\n3. View Services\n4. View Patients\n5. View Admins\n"
        << "6. Create Object\n7. Assign Object\n8. Remove Object\n9. Compare\n10. Delete Object\n"
        << "11. Check Service\n12. Find Admin\n13. Polymorphic Overview\n14. Book Appt\n15. View Appts\n"
        << "16. Cancel Appt\n17. Collection Demo\n0. Exit\nChoice: ";
}

int main() {
    ClinicData data;
    fillDemoData(data);

    int mainChoice = -1;
    while (mainChoice != 0) {
        printMainMenu();
        mainChoice = readMenuChoice();

        switch (mainChoice) {
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
        case 0:
        case -1:
            break;
        default:
            cout << "[ERROR] Unknown menu item.\n";
            break;
        }
    }
    return 0;
}
