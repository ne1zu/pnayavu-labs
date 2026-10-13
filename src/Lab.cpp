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

bool isSeniorDoctor(const Doctor& d) { return d.getExperience() >= 10; }
bool isMidDoctor(const Doctor& d) { return d.getExperience() >= 5; }
bool isCheapService(const Service& s) { return s.getPrice() < 40.0f; }
bool isAdultPatient(const Patient& p) { return p.isAdult(); }
bool isMorningAppointment(const Appointment& a) { return a.classify() == "Morning"; }
bool isNotMorningAppointment(const Appointment& a) { return a.classify() != "Morning"; }
bool isEvenNumber(const int& n) { return n % 2 == 0; }
bool isLongWord(const string& w) { return w.size() > 4; }

void printAllDoctors(const vector<shared_ptr<Doctor>>& doctorList) {
    if (doctorList.empty()) { cout << "Empty\n"; return; }
    int count = static_cast<int>(doctorList.size());
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << *doctorList[i] << " [" << doctorList[i]->classify() << "]\n";
}
void printAllServices(const vector<shared_ptr<Service>>& serviceList) {
    if (serviceList.empty()) { cout << "Empty\n"; return; }
    int count = static_cast<int>(serviceList.size());
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << *serviceList[i] << " [" << serviceList[i]->classify() << "]\n";
}
void printAllPatients(const vector<shared_ptr<Patient>>& patientList) {
    if (patientList.empty()) { cout << "Empty\n"; return; }
    int count = static_cast<int>(patientList.size());
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << *patientList[i] << " [" << patientList[i]->classify() << "]\n";
}
void printAllAdministrators(const vector<shared_ptr<Administrator>>& adminList) {
    if (adminList.empty()) { cout << "Empty\n"; return; }
    int count = static_cast<int>(adminList.size());
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << *adminList[i] << " [" << adminList[i]->classify() << "]\n";
}
void printAllFilials(const vector<shared_ptr<Filial>>& filialList) {
    if (filialList.empty()) { cout << "Empty\n"; return; }
    int count = static_cast<int>(filialList.size());
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << filialList[i]->getName() << " (" << filialList[i]->getAddress() << ")\n";
}

shared_ptr<Patient> findExistingPatient(const vector<shared_ptr<Patient>>& list, const Patient& candidate) {
    for (const auto& item : list) if (*item == candidate) return item; return nullptr;
}
shared_ptr<Doctor> findExistingDoctor(const vector<shared_ptr<Doctor>>& list, const Doctor& candidate) {
    for (const auto& item : list) if (*item == candidate) return item; return nullptr;
}
shared_ptr<Service> findExistingService(const vector<shared_ptr<Service>>& list, const Service& candidate) {
    for (const auto& item : list) if (*item == candidate) return item; return nullptr;
}
shared_ptr<Administrator> findExistingAdministrator(const vector<shared_ptr<Administrator>>& list, const Administrator& candidate) {
    for (const auto& item : list) if (*item == candidate) return item; return nullptr;
}

void interactiveCompare(const vector<shared_ptr<Doctor>>& doctors,
    const vector<shared_ptr<Service>>& services,
    const vector<shared_ptr<Patient>>& patients,
    const vector<shared_ptr<Administrator>>& administrators) {

    cout << "\n1. Compare Doctors\n";
    cout << "2. Compare Services\n";
    cout << "3. Compare Patients\n";
    cout << "4. Compare Administrators\n";
    cout << "0. Cancel\nChoice: ";

    int choice;
    cin >> choice;

    if (choice == 1) {
        int count = static_cast<int>(doctors.size());
        if (count < 2) return;
        printAllDoctors(doctors);
        int idx1, idx2;
        cin >> idx1 >> idx2; idx1--; idx2--;

        if (idx1 >= 0 && idx1 < count && idx2 >= 0 && idx2 < count) {
            auto a1 = doctors[idx1];
            auto a2 = doctors[idx2];
            if (*a1 == *a2) cout << "Same\n";
            else if (*a1 > *a2) cout << a1->getName() << " is >\n";
            else if (*a1 < *a2) cout << a2->getName() << " is >\n";
            else cout << "Equal\n";
        }
    }
    else if (choice == 2) {
        int count = static_cast<int>(services.size());
        if (count < 2) return;
        printAllServices(services);
        int idx1, idx2;
        cin >> idx1 >> idx2; idx1--; idx2--;

        if (idx1 >= 0 && idx1 < count && idx2 >= 0 && idx2 < count) {
            auto a1 = services[idx1];
            auto a2 = services[idx2];
            if (*a1 == *a2) cout << "Same\n";
            else if (*a1 > *a2) cout << a1->getName() << " is >\n";
            else if (*a1 < *a2) cout << a2->getName() << " is >\n";
            else cout << "Equal\n";
        }
    }
    else if (choice == 3) {
        int count = static_cast<int>(patients.size());
        if (count < 2) return;
        printAllPatients(patients);
        int idx1, idx2;
        cin >> idx1 >> idx2; idx1--; idx2--;

        if (idx1 >= 0 && idx1 < count && idx2 >= 0 && idx2 < count) {
            auto a1 = patients[idx1];
            auto a2 = patients[idx2];
            if (*a1 == *a2) cout << "Same\n";
            else if (*a1 > *a2) cout << a1->getName() << " is >\n";
            else if (*a1 < *a2) cout << a2->getName() << " is >\n";
            else cout << "Equal\n";
        }
    }
    else if (choice == 4) {
        int count = static_cast<int>(administrators.size());
        if (count < 2) return;
        printAllAdministrators(administrators);
        int idx1, idx2;
        cin >> idx1 >> idx2; idx1--; idx2--;

        if (idx1 >= 0 && idx1 < count && idx2 >= 0 && idx2 < count) {
            auto a1 = administrators[idx1];
            auto a2 = administrators[idx2];
            if (*a1 == *a2) cout << "Same\n";
            else if (*a1 > *a2) cout << a1->getName() << " is >\n";
            else if (*a1 < *a2) cout << a2->getName() << " is >\n";
            else cout << "Equal\n";
        }
    }
}

bool readInt(int& value) {
    if (cin >> value) return true;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return false;
}

shared_ptr<Filial> chooseFilial(const vector<shared_ptr<Filial>>& filials) {
    printAllFilials(filials);
    cout << "Select Branch index: ";
    int bIdx;
    if (!readInt(bIdx)) return nullptr;
    bIdx--;
    int filialsCount = static_cast<int>(filials.size());
    if (bIdx < 0 || bIdx >= filialsCount) return nullptr;
    return filials[bIdx];
}

template <typename T>
shared_ptr<T> chooseFrom(const string& title, const Collection<T>& collection) {
    if (collection.isEmpty()) return nullptr;
    collection.print();
    int idx;
    if (!readInt(idx)) return nullptr;
    shared_ptr<T> chosen = collection.get(idx - 1);
    return chosen;
}

void bookAppointment(const vector<shared_ptr<Filial>>& filials) {
    shared_ptr<Filial> filial = chooseFilial(filials);
    if (!filial) return;

    shared_ptr<Patient> patient = chooseFrom("Patient", filial->getPatients());
    if (!patient) return;
    shared_ptr<Doctor> doctor = chooseFrom("Doctor", filial->getDoctors());
    if (!doctor) return;
    shared_ptr<Service> service = chooseFrom("Service", filial->getServices());
    if (!service) return;

    int day, month, year, hour, minute;
    cout << "Enter date (day month year): ";
    if (!readInt(day) || !readInt(month) || !readInt(year)) return;
    if (!Appointment::isValidDate(day, month, year)) return;
    cout << "Enter start time (hour minute): ";
    if (!readInt(hour) || !readInt(minute)) return;
    if (!Appointment::isValidTime(hour, minute)) return;

    shared_ptr<Appointment> appointment = make_shared<Appointment>(
        patient, doctor, service, filial->getName(), day, month, year, hour, minute);

    *filial += appointment;
}

void showAppointments(const vector<shared_ptr<Filial>>& filials) {
    shared_ptr<Filial> filial = chooseFilial(filials);
    if (!filial) return;

    Collection<Appointment> all = filial->getAppointments();
    cout << "1. All\n2. By patient\n3. By doctor\n4. Latest\n5. Morning count\nChoice: ";
    int sub;
    if (!readInt(sub)) return;

    if (sub == 1) {
        all.print(cout, "  ");
    }
    else if (sub == 2) {
        shared_ptr<Patient> patient = chooseFrom("Patient", filial->getPatients());
        if (!patient) return;

        Collection<Appointment> found;
        for (const auto& a : all) {
            if (*a->getPatient() == *patient) {
                found.add(a);
            }
        }
        found.print(cout, "  ");
    }
    else if (sub == 3) {
        shared_ptr<Doctor> doctor = chooseFrom("Doctor", filial->getDoctors());
        if (!doctor) return;

        Collection<Appointment> found;
        for (const auto& a : all) {
            if (*a->getDoctor() == *doctor) {
                found.add(a);
            }
        }
        found.print(cout, "  ");
    }
    else if (sub == 4) {
        shared_ptr<Appointment> latest = findMax(all);
        if (latest) cout << "-> Latest: " << *latest << "\n";
    }
    else if (sub == 5) {
        int morning = countIf(all, isMorningAppointment);
        cout << "-> Morning: " << morning << "\n";
    }
}

void cancelAppointment(const vector<shared_ptr<Filial>>& filials) {
    shared_ptr<Filial> filial = chooseFilial(filials);
    if (!filial) return;

    shared_ptr<Appointment> appointment = chooseFrom("Appointment", filial->getAppointments());
    if (!appointment) return;

    *filial -= appointment;
}

void collectionDemo(const vector<shared_ptr<Filial>>& filials) {
    shared_ptr<Filial> filial = chooseFilial(filials);
    if (!filial) return;

   
    Collection<int> numbers;
    for (int i = 1; i <= 5; i++) numbers.add(make_shared<int>(i * 10)); 
    Collection<string> words;
    words.add(make_shared<string>("apple"));
    words.add(make_shared<string>("banana"));

    int choice = -1;
    while (choice != 0) {
        cout << "\n=== COLLECTION<T> FULL DEMO MENU (" << filial->getName() << ") ===\n";
        cout << "1. Add & Size methods (add, getCount, isEmpty, print)\n";
        cout << "2. Access & Search methods (get, at, indexOf, contains, find)\n";
        cout << "3. Removal methods (removeAt, remove, removeIf, clear)\n";
        cout << "4. Template Algorithms (findFirst, findAll, countIf, findMax)\n";
        cout << "5. Domain Independence (Collection<int>, Collection<string>)\n";
        cout << "0. Go back\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            cout << "\n--- 1. Add & Size Methods ---\n";
            Collection<Doctor> demoDocs;

            cout << "Calling isEmpty()... -> " << (demoDocs.isEmpty() ? "True" : "False") << "\n";
            cout << "Calling add()... Adding 2 temporary doctors.\n";
            demoDocs.add(make_shared<Doctor>("Andrei Ivanov", "Therapist", 5));
            demoDocs.add(make_shared<Doctor>("Sergei Krug", "Surgeon", 10));

            cout << "Calling isEmpty()... -> " << (demoDocs.isEmpty() ? "True" : "False") << "\n";
            cout << "Calling getCount().. -> " << demoDocs.getCount() << "\n";
            cout << "Calling print()..... ->\n";
            demoDocs.print(cout, "  ");
        }
        else if (choice == 2) {
            cout << "\n--- 2. Access & Search Methods ---\n";
            Collection<Doctor> docs = filial->getDoctors();
            if (docs.isEmpty()) { cout << "[ERROR] Need doctors in branch to test search!\n"; continue; }

            auto target = docs.get(0); 
            cout << "Calling get(0)........ -> " << target->getName() << "\n";

            cout << "Calling at(100)....... -> ";
            try {
                docs.at(100);
            }
            catch (const out_of_range& e) {
                cout << "Exception safely caught! (" << e.what() << ")\n";
            }

            cout << "Calling indexOf()..... -> " << docs.indexOf(*target) << "\n";
            cout << "Calling contains().... -> " << (docs.contains(*target) ? "True" : "False") << "\n";

            auto found = docs.find(*target);
            cout << "Calling find()........ -> " << (found ? "Object successfully found in memory" : "Not found") << "\n";
        }
        else if (choice == 3) {
            cout << "\n--- 3. Removal Methods (Working on a COPY of the list) ---\n";
            Collection<Doctor> demoDocs = filial->getDoctors(); 
            if (demoDocs.isEmpty()) { cout << "[ERROR] Need doctors in branch to test removal!\n"; continue; }

            cout << "Initial count: " << demoDocs.getCount() << "\n";

            demoDocs.removeAt(0);
            cout << "After removeAt(0).... count is: " << demoDocs.getCount() << "\n";

            if (!demoDocs.isEmpty()) {
                auto target = demoDocs.get(0);
                demoDocs.remove(*target);
                cout << "After remove(value).. count is: " << demoDocs.getCount() << "\n";
            }

            int removedByIf = demoDocs.removeIf(isSeniorDoctor);
            cout << "After removeIf(Senior) removed " << removedByIf
                << " doctors. Remaining: " << demoDocs.getCount() << "\n";

            demoDocs.clear();
            cout << "After clear()........ isEmpty() is -> " << (demoDocs.isEmpty() ? "True" : "False") << "\n";
        }
        else if (choice == 4) {
            cout << "\n--- 4. Template Algorithms ---\n";
            Collection<Doctor> docs = filial->getDoctors();

            auto senior = findFirst(docs, isSeniorDoctor);
            cout << "findFirst(isSeniorDoctor) ... -> " << (senior ? senior->getName() : "None") << "\n";

            Collection<Doctor> midDocs = findAll(docs, isMidDoctor);
            cout << "findAll(isMidDoctor)......... -> Found " << midDocs.getCount() << " doctors\n";

            cout << "countIf(isSeniorDoctor)...... -> " << countIf(docs, isSeniorDoctor) << "\n";

            auto best = findMax(docs); 
            cout << "findMax() [Highest exp]...... -> " << (best ? best->getName() : "None") << "\n";
        }
        else if (choice == 5) {
            cout << "\n--- 5. Domain Independence (Primitive Types) ---\n";

            cout << "Collection<int> numbers:\n";
            numbers.print(cout, "  ");
            cout << "findMax() in numbers ........ -> " << *findMax(numbers) << "\n";
            cout << "countIf(isEvenNumber) ....... -> " << countIf(numbers, isEvenNumber) << "\n";

            cout << "\nCollection<string> words:\n";
            words.print(cout, "  ");
            auto longW = findFirst(words, isLongWord);
            if (longW) cout << "findFirst(isLongWord) ....... -> " << *longW << "\n";
            else cout << "findFirst(isLongWord) ....... -> None\n";
        }
    }
}
int main() {
    vector<shared_ptr<Service>> globalServices;
    vector<shared_ptr<Doctor>> globalDoctors;
    vector<shared_ptr<Patient>> globalPatients;
    vector<shared_ptr<Administrator>> globalAdministrators;
    vector<shared_ptr<Filial>> globalFilials;

    globalServices.push_back(make_shared<Service>("Therapy Session", 25.0f, 15));
    globalServices.push_back(make_shared<Service>("Massage", 50.0f, 30));
    globalServices.push_back(make_shared<Service>("MRI Scan", 150.0f, 45));
    globalServices.push_back(make_shared<Service>("Blood Test", 10.0f, 5));
    globalServices.push_back(make_shared<Service>("Dental Surgery", 200.0f, 60));

    globalDoctors.push_back(make_shared<Doctor>("Ivanov Ivan", "Therapist", 5));
    globalDoctors.push_back(make_shared<Doctor>("House Gregory", "Diagnostician", 20));
    globalDoctors.push_back(make_shared<Doctor>("Watson John", "Surgeon", 8));
    globalDoctors.push_back(make_shared<Doctor>("Dorian John", "Intern", 1));
    globalDoctors.push_back(make_shared<Doctor>("Quinn Medicine", "Pediatrician", 12));

    globalPatients.push_back(make_shared<Patient>("Smirnov Oleg", 34, "+375291112233"));
    globalPatients.push_back(make_shared<Patient>("Potter Harry", 15, "+375290000001"));
    globalPatients.push_back(make_shared<Patient>("Wayne Bruce", 45, "+375290000002"));
    globalPatients.push_back(make_shared<Patient>("Granger Hermione", 17, "+375290000003"));
    globalPatients.push_back(make_shared<Patient>("Stark Tony", 50, "+375290000004"));

    globalAdministrators.push_back(make_shared<Administrator>("Petrov Petr", "Head Physician", 25, 50));
    globalAdministrators.push_back(make_shared<Administrator>("Scott Michael", "Regional Manager", 15, 30));
    globalAdministrators.push_back(make_shared<Administrator>("Swanson Ron", "Director", 5, 10));
    globalAdministrators.push_back(make_shared<Administrator>("Moss Maurice", "IT Support", 2, 5));

    globalFilials.push_back(make_shared<Filial>("Central Clinic", "Main st 10"));
    globalFilials.push_back(make_shared<Filial>("North Branch", "Cold st 1"));
    globalFilials.push_back(make_shared<Filial>("South Branch", "Sunny blvd 99"));

    *globalFilials[0] += globalDoctors[0];
    *globalFilials[0] += globalDoctors[1];
    *globalFilials[0] += globalServices[0];
    *globalFilials[0] += globalServices[2];
    *globalFilials[0] += globalPatients[0];
    *globalFilials[0] += globalPatients[2];
    *globalFilials[0] += globalAdministrators[0];

    *globalFilials[1] += globalDoctors[3];
    *globalFilials[1] += globalServices[3];
    *globalFilials[1] += globalPatients[1];
    *globalFilials[1] += globalAdministrators[1];
    *globalFilials[1] += globalAdministrators[3];

    *globalFilials[2] += globalDoctors[2];
    *globalFilials[2] += globalDoctors[4];
    *globalFilials[2] += globalServices[1];
    *globalFilials[2] += globalServices[4];
    *globalFilials[2] += globalPatients[3];
    *globalFilials[2] += globalPatients[4];
    *globalFilials[2] += globalAdministrators[2];

    int mainChoice = -1;
    while (mainChoice != 0) {
        cout << "\n1. View Branches\n2. View Doctors\n3. View Services\n4. View Patients\n5. View Admins\n6. Create Object\n7. Assign Object\n8. Remove Object\n9. Compare\n10. Delete Object\n11. Check Service\n12. Find Admin\n13. Polymorphic Overview\n14. Book Appt\n15. View Appts\n16. Cancel Appt\n17. Collection Demo\n0. Exit\nChoice: ";
        cin >> mainChoice;

        switch (mainChoice) {
        case 1:
            for (const auto& currentFilial : globalFilials) cout << *currentFilial;
            break;
        case 2: printAllDoctors(globalDoctors); break;
        case 3: printAllServices(globalServices); break;
        case 4: printAllPatients(globalPatients); break;
        case 5: printAllAdministrators(globalAdministrators); break;

        case 6: {
            cout << "1. Patient 2. Doctor 3. Service 4. Administrator\nChoice: ";
            int sub; cin >> sub;
            if (sub == 1) {
                Patient p; cin >> p;
                if (!findExistingPatient(globalPatients, p)) globalPatients.push_back(make_shared<Patient>(p));
            }
            else if (sub == 2) {
                Doctor d; cin >> d;
                if (!findExistingDoctor(globalDoctors, d)) globalDoctors.push_back(make_shared<Doctor>(d));
            }
            else if (sub == 3) {
                Service s; cin >> s;
                if (!findExistingService(globalServices, s)) globalServices.push_back(make_shared<Service>(s));
            }
            else if (sub == 4) {
                Administrator a; cin >> a;
                if (!findExistingAdministrator(globalAdministrators, a)) globalAdministrators.push_back(make_shared<Administrator>(a));
            }
            break;
        }
        case 7: {
            printAllFilials(globalFilials);
            int bIdx; cin >> bIdx; bIdx--;
            if (bIdx >= 0 && bIdx < static_cast<int>(globalFilials.size())) {
                cout << "1. Doctor 2. Service 3. Patient 4. Administrator\nChoice: ";
                int sub; cin >> sub;
                if (sub == 1) {
                    printAllDoctors(globalDoctors); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < static_cast<int>(globalDoctors.size())) *globalFilials[bIdx] += globalDoctors[idx];
                }
                else if (sub == 2) {
                    printAllServices(globalServices); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < static_cast<int>(globalServices.size())) *globalFilials[bIdx] += globalServices[idx];
                }
                else if (sub == 3) {
                    printAllPatients(globalPatients); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < static_cast<int>(globalPatients.size())) *globalFilials[bIdx] += globalPatients[idx];
                }
                else if (sub == 4) {
                    printAllAdministrators(globalAdministrators); int aIdx; cin >> aIdx; aIdx--;
                    if (aIdx >= 0 && aIdx < static_cast<int>(globalAdministrators.size())) *globalFilials[bIdx] += globalAdministrators[aIdx];
                }
            }
            break;
        }
        case 8: {
            printAllFilials(globalFilials);
            int bIdx; cin >> bIdx; bIdx--;
            if (bIdx >= 0 && bIdx < static_cast<int>(globalFilials.size())) {
                cout << "1. Doctor 2. Service 3. Patient 4. Administrator\nChoice: ";
                int sub; cin >> sub;
                if (sub == 1) {
                    printAllDoctors(globalDoctors); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < static_cast<int>(globalDoctors.size())) *globalFilials[bIdx] -= globalDoctors[idx];
                }
                else if (sub == 2) {
                    printAllServices(globalServices); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < static_cast<int>(globalServices.size())) *globalFilials[bIdx] -= globalServices[idx];
                }
                else if (sub == 3) {
                    printAllPatients(globalPatients); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < static_cast<int>(globalPatients.size())) *globalFilials[bIdx] -= globalPatients[idx];
                }
                else if (sub == 4) {
                    printAllAdministrators(globalAdministrators); int aIdx; cin >> aIdx; aIdx--;
                    if (aIdx >= 0 && aIdx < static_cast<int>(globalAdministrators.size())) *globalFilials[bIdx] -= globalAdministrators[aIdx];
                }
            }
            break;
        }
        case 9:
            interactiveCompare(globalDoctors, globalServices, globalPatients, globalAdministrators);
            break;

        case 10: {
            cout << "1. Doctor 2. Service 3. Patient 4. Administrator\nChoice: ";
            int sub; cin >> sub;
            if (sub == 1) {
                printAllDoctors(globalDoctors); int idx; cin >> idx; idx--;
                if (idx >= 0 && idx < static_cast<int>(globalDoctors.size())) {
                    auto toDel = globalDoctors[idx];
                    for (auto& f : globalFilials) *f -= toDel;
                    globalDoctors.erase(globalDoctors.begin() + idx);
                }
            }
            else if (sub == 2) {
                printAllServices(globalServices); int idx; cin >> idx; idx--;
                if (idx >= 0 && idx < static_cast<int>(globalServices.size())) {
                    auto toDel = globalServices[idx];
                    for (auto& f : globalFilials) *f -= toDel;
                    globalServices.erase(globalServices.begin() + idx);
                }
            }
            else if (sub == 3) {
                printAllPatients(globalPatients); int idx; cin >> idx; idx--;
                if (idx >= 0 && idx < static_cast<int>(globalPatients.size())) {
                    auto toDel = globalPatients[idx];
                    for (auto& f : globalFilials) *f -= toDel;
                    globalPatients.erase(globalPatients.begin() + idx);
                }
            }
            else if (sub == 4) {
                printAllAdministrators(globalAdministrators); int aIdx; cin >> aIdx; aIdx--;
                if (aIdx >= 0 && aIdx < static_cast<int>(globalAdministrators.size())) {
                    auto toDel = globalAdministrators[aIdx];
                    for (auto& f : globalFilials) *f -= toDel;
                    globalAdministrators.erase(globalAdministrators.begin() + aIdx);
                }
            }
            break;
        }
        case 11: {
            printAllFilials(globalFilials); int bIdx; cin >> bIdx; bIdx--;
            if (bIdx >= 0 && bIdx < static_cast<int>(globalFilials.size())) {
                printAllServices(globalServices); int sIdx; cin >> sIdx; sIdx--;
                if (sIdx >= 0 && sIdx < static_cast<int>(globalServices.size())) {
                    bool available = isServiceAvailable(*globalFilials[bIdx], *globalServices[sIdx]);
                    cout << (available ? "Yes\n" : "No\n");
                }
            }
            break;
        }
        case 12: {
            printAllFilials(globalFilials); int bIdx; cin >> bIdx; bIdx--;
            if (bIdx >= 0 && bIdx < static_cast<int>(globalFilials.size())) {
                string pos; cin >> ws; getline(cin, pos);
                auto found = findAdministratorByPosition(*globalFilials[bIdx], pos);
                if (found) cout << *found << "\n";
            }
            break;
        }
        case 13: {
            printAllFilials(globalFilials); int bIdx; cin >> bIdx; bIdx--;
            if (bIdx >= 0 && bIdx < static_cast<int>(globalFilials.size())) {
                vector<shared_ptr<Person>> all = globalFilials[bIdx]->getAllEntities();
                for (const auto& entity : all) {
                    cout << "[" << entity->getEntityType() << "] ";
                    entity->printInfo(cout);
                    cout << " -> " << entity->classify() << "\n";
                }
            }
            break;
        }

        case 14: bookAppointment(globalFilials); break;
        case 15: showAppointments(globalFilials); break;
        case 16: cancelAppointment(globalFilials); break;
        case 17: collectionDemo(globalFilials); break;

        case 0: break;
        }
    }
    return 0;
}