#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "service.h"
#include "doctor.h"
#include "patient.h"
#include "filial.h"
#include "clinic_record.h"

using namespace std;

void printAllDoctors(const vector<shared_ptr<Doctor>>& doctorList) {
    if (doctorList.empty()) { cout << "(Database is empty)\n"; return; }
    int count = doctorList.size();
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << *doctorList[i] << " [" << doctorList[i]->classify() << "]\n";
}
void printAllServices(const vector<shared_ptr<Service>>& serviceList) {
    if (serviceList.empty()) { cout << "(Database is empty)\n"; return; }
    int count = serviceList.size();
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << *serviceList[i] << " [" << serviceList[i]->classify() << "]\n";
}
void printAllPatients(const vector<shared_ptr<Patient>>& patientList) {
    if (patientList.empty()) { cout << "(Database is empty)\n"; return; }
    int count = patientList.size();
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << *patientList[i] << " [" << patientList[i]->classify() << "]\n";
}
void printAllFilials(const vector<shared_ptr<Filial>>& filialList) {
    if (filialList.empty()) { cout << "(Database is empty)\n"; return; }
    int count = filialList.size();
    for (int i = 0; i < count; i++)
        cout << i + 1 << ". " << filialList[i]->getName() << " (" << filialList[i]->getAddress() << ")\n";
}


shared_ptr<Patient> findExistingPatient(const vector<shared_ptr<Patient>>& list, const Patient& candidate) {
    int count = list.size();
    for (int i = 0; i < count; i++) {
        if (*list[i] == candidate) return list[i];
    }
    return nullptr;
}
shared_ptr<Doctor> findExistingDoctor(const vector<shared_ptr<Doctor>>& list, const Doctor& candidate) {
    int count = list.size();
    for (int i = 0; i < count; i++) {
        if (*list[i] == candidate) return list[i];
    }
    return nullptr;
}
shared_ptr<Service> findExistingService(const vector<shared_ptr<Service>>& list, const Service& candidate) {
    int count = list.size();
    for (int i = 0; i < count; i++) {
        if (*list[i] == candidate) return list[i];
    }
    return nullptr;
}

void interactiveCompare(const vector<shared_ptr<Doctor>>& doctors,
    const vector<shared_ptr<Service>>& services,
    const vector<shared_ptr<Patient>>& patients) {

    cout << "\n=== LOGICAL COMPARISON MENU ===\n";
    cout << "What business logic do you want to test?\n";
    cout << "1. Compare Doctors (Determine seniority for a complex surgery)\n";
    cout << "2. Compare Services (Help a patient choose based on budget)\n";
    cout << "3. Compare Patients (Determine queue priority based on age)\n";
    cout << "0. Cancel\nChoice: ";

    int choice;
    cin >> choice;

    if (choice == 1) {
        int doctorsCount = doctors.size();
        if (doctorsCount < 2) { cout << "Need at least 2 doctors!\n"; return; }
        cout << "\n--- Select 2 Doctors to compare ---\n";
        printAllDoctors(doctors);
        int idx1, idx2;
        cout << "Enter first doctor number: "; cin >> idx1; idx1--;
        cout << "Enter second doctor number: "; cin >> idx2; idx2--;

        if (idx1 >= 0 && idx1 < doctorsCount && idx2 >= 0 && idx2 < doctorsCount) {
            auto d1 = doctors[idx1];
            auto d2 = doctors[idx2];
            cout << "\n[RESULT] Comparing " << d1->getFio() << " and " << d2->getFio() << ":\n";

            if (*d1 == *d2) {
                cout << "-> DUPLICATE RECORD: these are the same doctor (same name), entered separately!\n";
            }
            else if (*d1 > *d2) {
                cout << "-> LOGIC: " << d1->getFio() << " is MORE experienced. Assign them as the Head Surgeon.\n";
            }
            else if (*d1 < *d2) {
                cout << "-> LOGIC: " << d2->getFio() << " is MORE experienced. Assign them as the Head Surgeon.\n";
            }
            else {
                cout << "-> LOGIC: Both have equal experience. They can assist each other.\n";
            }
        }
        else { cout << "Invalid selection!\n"; }
    }
    else if (choice == 2) {
        int servicesCount = services.size();
        if (servicesCount < 2) { cout << "Need at least 2 services!\n"; return; }
        cout << "\n--- Select 2 Services to compare ---\n";
        printAllServices(services);
        int idx1, idx2;
        cout << "Enter first service number: "; cin >> idx1; idx1--;
        cout << "Enter second service number: "; cin >> idx2; idx2--;

        if (idx1 >= 0 && idx1 < servicesCount && idx2 >= 0 && idx2 < servicesCount) {
            auto s1 = services[idx1];
            auto s2 = services[idx2];
            cout << "\n[RESULT] Comparing '" << s1->getName() << "' and '" << s2->getName() << "':\n";

            if (*s1 == *s2) {
                cout << "-> DUPLICATE RECORD: this service is registered twice under the same name.\n";
            }
            else if (*s1 > *s2) {
                cout << "-> LOGIC: '" << s1->getName() << "' is PREMIUM. Recommend '" << s2->getName() << "' for a tight budget.\n";
            }
            else if (*s1 < *s2) {
                cout << "-> LOGIC: '" << s2->getName() << "' is PREMIUM. Recommend '" << s1->getName() << "' for a tight budget.\n";
            }
            else {
                cout << "-> LOGIC: They cost the same. The patient can choose either.\n";
            }
        }
        else { cout << "Invalid selection!\n"; }
    }
    else if (choice == 3) {
        int patientsCount = patients.size();
        if (patientsCount < 2) { cout << "Need at least 2 patients!\n"; return; }
        cout << "\n--- Select 2 Patients to compare ---\n";
        printAllPatients(patients);
        int idx1, idx2;
        cout << "Enter first patient number: "; cin >> idx1; idx1--;
        cout << "Enter second patient number: "; cin >> idx2; idx2--;

        if (idx1 >= 0 && idx1 < patientsCount && idx2 >= 0 && idx2 < patientsCount) {
            auto p1 = patients[idx1];
            auto p2 = patients[idx2];
            cout << "\n[RESULT] Comparing " << p1->getFio() << " and " << p2->getFio() << ":\n";

            if (*p1 == *p2) {
                cout << "-> DUPLICATE RECORD: same phone number, this is the same patient entered twice!\n";
            }
            else if (*p1 > *p2) {
                cout << "-> LOGIC: " << p1->getFio() << " is OLDER. They get priority in the queue.\n";
            }
            else if (*p1 < *p2) {
                cout << "-> LOGIC: " << p2->getFio() << " is OLDER. They get priority in the queue.\n";
            }
            else {
                cout << "-> LOGIC: They are the same age. First come, first served.\n";
            }
        }
        else { cout << "Invalid selection!\n"; }
    }
}

int main() {
    vector<shared_ptr<Service>> globalServices;
    vector<shared_ptr<Doctor>> globalDoctors;
    vector<shared_ptr<Patient>> globalPatients;
    vector<shared_ptr<Filial>> globalFilials;

    globalServices.push_back(make_shared<Service>("Massage", 50.0f, 30));
    globalServices.push_back(make_shared<Service>("Therapist consultation", 25.5f, 15));
    globalServices.push_back(make_shared<Service>("Xray", 35.0f, 10));

    globalDoctors.push_back(make_shared<Doctor>("Ivanov Ivan", "Therapist", 5));
    globalDoctors.push_back(make_shared<Doctor>("Petrova Anna", "Surgeon", 15));
    globalDoctors.push_back(make_shared<Doctor>("Sidorov Petr", "Masseur", 3));

    globalPatients.push_back(make_shared<Patient>("Smirnov Oleg", 34, "+375291112233"));
    globalPatients.push_back(make_shared<Patient>("Volkova Maria", 65, "+375291112244"));
    globalPatients.push_back(make_shared<Patient>("Egorov Denis", 19, "+375291112255"));

    globalFilials.push_back(make_shared<Filial>("Central", "Main st 10", 3, 3, 3));
    globalFilials.push_back(make_shared<Filial>("North", "Peace st 25", 5, 5, 5));

    *globalFilials[0] += globalDoctors[0];
    *globalFilials[0] += globalDoctors[1];
    *globalFilials[0] += globalServices[0];
    *globalFilials[0] += globalPatients[0];

    int mainChoice = -1;
    while (mainChoice != 0) {
        cout << "\n===== CLINIC MANAGEMENT SYSTEM =====\n";
        cout << "1. View all Branches (with details)\n";
        cout << "2. View all Doctors\n";
        cout << "3. View all Services\n";
        cout << "4. View all Patients\n";
        cout << "-------------------------------------------\n";
        cout << "5. Create New Object (via >>)\n";
        cout << "6. Assign Object to Branch (via +=)\n";
        cout << "7. Remove Object from Branch (via -=)\n";
        cout << "8. INTERACTIVE LOGICAL COMPARISON (<, >, ==)\n";
        cout << "9. DELETE Object completely from Database\n";
        cout << "-------------------------------------------\n";
        cout << "10. Check Service availability at a Branch (friend function)\n";
        cout << "11. Find a Doctor by specialty at a Branch (friend function)\n";
        cout << "-------------------------------------------\n";
        cout << "12. Polymorphic Overview of a Branch (base-class pointers)\n";
        cout << "0. Exit\n";
        cout << "Select an option: ";
        cin >> mainChoice;

        switch (mainChoice) {
        case 1:
            for (const auto& currentFilial : globalFilials) {
                cout << *currentFilial;
                cout << "Doctor count: " << currentFilial->getDoctorCount()
                    << ", Service count: " << currentFilial->getServiceCount()
                    << ", Patient count: " << currentFilial->getPatientCount() << "\n";
            }
            break;
        case 2: cout << "\nDoctors:\n"; printAllDoctors(globalDoctors); break;
        case 3: cout << "\nServices:\n"; printAllServices(globalServices); break;
        case 4: cout << "\nPatients:\n"; printAllPatients(globalPatients); break;

        case 5: {
            cout << "What to create?\n1. Patient\n2. Doctor\n3. Service\nChoice: ";
            int sub; cin >> sub;

            if (sub == 1) {
                Patient p;
                cin >> p;
                auto existing = findExistingPatient(globalPatients, p);
                if (existing) {
                    cout << "-> Already registered: " << *existing
                        << "\n-> Reusing the existing record instead of creating a duplicate.\n";
                }
                else {
                    globalPatients.push_back(make_shared<Patient>(p));
                    cout << "Success! New patient registered.\n";
                }
            }
            else if (sub == 2) {
                Doctor d;
                cin >> d;
                auto existing = findExistingDoctor(globalDoctors, d);
                if (existing) {
                    cout << "-> Already registered: " << *existing
                        << "\n-> Reusing the existing record instead of creating a duplicate.\n";
                }
                else {
                    globalDoctors.push_back(make_shared<Doctor>(d));
                    cout << "Success! New doctor registered.\n";
                }
            }
            else if (sub == 3) {
                Service s;
                cin >> s;
                auto existing = findExistingService(globalServices, s);
                if (existing) {
                    cout << "-> Already registered: " << *existing
                        << "\n-> Reusing the existing record instead of creating a duplicate.\n";
                }
                else {
                    globalServices.push_back(make_shared<Service>(s));
                    cout << "Success! New service registered.\n";
                }
            }
            break;
        }
        case 6: {
            printAllFilials(globalFilials);
            cout << "Select Branch index: ";
            int bIdx; cin >> bIdx; bIdx--;
            int filialsCount = globalFilials.size();

            if (bIdx >= 0 && bIdx < filialsCount) {
                cout << "Assign:\n1. Doctor\n2. Service\n3. Patient\nChoice: ";
                int sub; cin >> sub;

                if (sub == 1) {
                    printAllDoctors(globalDoctors);
                    cout << "Select Doctor index: ";
                    int dIdx; cin >> dIdx; dIdx--;
                    int doctorsCount = globalDoctors.size();
                    if (dIdx >= 0 && dIdx < doctorsCount) *globalFilials[bIdx] += globalDoctors[dIdx];
                }
                else if (sub == 2) {
                    printAllServices(globalServices);
                    cout << "Select Service index: ";
                    int sIdx; cin >> sIdx; sIdx--;
                    int servicesCount = globalServices.size();
                    if (sIdx >= 0 && sIdx < servicesCount) *globalFilials[bIdx] += globalServices[sIdx];
                }
                else if (sub == 3) {
                    printAllPatients(globalPatients);
                    cout << "Select Patient index: ";
                    int pIdx; cin >> pIdx; pIdx--;
                    int patientsCount = globalPatients.size();
                    if (pIdx >= 0 && pIdx < patientsCount) *globalFilials[bIdx] += globalPatients[pIdx];
                }
            }
            else { cout << "Invalid Branch index!\n"; }
            break;
        }
        case 7: {
            printAllFilials(globalFilials);
            cout << "Select Branch index: ";
            int bIdx; cin >> bIdx; bIdx--;
            int filialsCount = globalFilials.size();

            if (bIdx >= 0 && bIdx < filialsCount) {
                cout << "Remove:\n1. Doctor\n2. Service\n3. Patient\nChoice: ";
                int sub; cin >> sub;

                if (sub == 1) {
                    printAllDoctors(globalDoctors);
                    cout << "Select Doctor index to remove: ";
                    int dIdx; cin >> dIdx; dIdx--;
                    int doctorsCount = globalDoctors.size();
                    if (dIdx >= 0 && dIdx < doctorsCount) *globalFilials[bIdx] -= globalDoctors[dIdx];
                }
                else if (sub == 2) {
                    printAllServices(globalServices);
                    cout << "Select Service index to remove: ";
                    int sIdx; cin >> sIdx; sIdx--;
                    int servicesCount = globalServices.size();
                    if (sIdx >= 0 && sIdx < servicesCount) *globalFilials[bIdx] -= globalServices[sIdx];
                }
                else if (sub == 3) {
                    printAllPatients(globalPatients);
                    cout << "Select Patient index to remove: ";
                    int pIdx; cin >> pIdx; pIdx--;
                    int patientsCount = globalPatients.size();
                    if (pIdx >= 0 && pIdx < patientsCount) *globalFilials[bIdx] -= globalPatients[pIdx];
                }
            }
            else { cout << "Invalid Branch index!\n"; }
            break;
        }
        case 8:
            interactiveCompare(globalDoctors, globalServices, globalPatients);
            break;

        case 9: {
            cout << "What to permanently delete from Database?\n1. Doctor\n2. Service\n3. Patient\nChoice: ";
            int sub; cin >> sub;
            cout << "\n(Note: The object will be automatically removed from ALL branches first)\n";

            if (sub == 1) {
                printAllDoctors(globalDoctors);
                cout << "Select Doctor index to delete: ";
                int dIdx; cin >> dIdx; dIdx--;
                int doctorsCount = globalDoctors.size();
                if (dIdx >= 0 && dIdx < doctorsCount) {
                    auto toDelete = globalDoctors[dIdx];
                    for (auto& f : globalFilials) { *f -= toDelete; }
                    globalDoctors.erase(globalDoctors.begin() + dIdx);
                    cout << "-> Doctor permanently deleted from the system.\n";
                }
                else { cout << "Invalid index!\n"; }
            }
            else if (sub == 2) {
                printAllServices(globalServices);
                cout << "Select Service index to delete: ";
                int sIdx; cin >> sIdx; sIdx--;
                int servicesCount = globalServices.size();
                if (sIdx >= 0 && sIdx < servicesCount) {
                    auto toDelete = globalServices[sIdx];
                    for (auto& f : globalFilials) { *f -= toDelete; }
                    globalServices.erase(globalServices.begin() + sIdx);
                    cout << "-> Service permanently deleted from the system.\n";
                }
                else { cout << "Invalid index!\n"; }
            }
            else if (sub == 3) {
                printAllPatients(globalPatients);
                cout << "Select Patient index to delete: ";
                int pIdx; cin >> pIdx; pIdx--;
                int patientsCount = globalPatients.size();
                if (pIdx >= 0 && pIdx < patientsCount) {
                    auto toDelete = globalPatients[pIdx];
                    for (auto& f : globalFilials) { *f -= toDelete; }
                    globalPatients.erase(globalPatients.begin() + pIdx);
                    cout << "-> Patient permanently deleted from the system.\n";
                }
                else { cout << "Invalid index!\n"; }
            }
            else { cout << "Invalid choice!\n"; }
            break;
        }

        case 10: {
            printAllFilials(globalFilials);
            cout << "Select Branch index: ";
            int bIdx; cin >> bIdx; bIdx--;
            int filialsCount = globalFilials.size();
            if (bIdx < 0 || bIdx >= filialsCount) { cout << "Invalid Branch index!\n"; break; }

            printAllServices(globalServices);
            cout << "Select Service index to check: ";
            int sIdx; cin >> sIdx; sIdx--;
            int servicesCount = globalServices.size();
            if (sIdx < 0 || sIdx >= servicesCount) { cout << "Invalid Service index!\n"; break; }

            bool available = isServiceAvailable(*globalFilials[bIdx], *globalServices[sIdx]);
            cout << "-> '" << globalServices[sIdx]->getName() << "' at \""
                << globalFilials[bIdx]->getName() << "\": "
                << (available ? "AVAILABLE, can be booked here.\n" : "NOT offered at this branch.\n");
            break;
        }
        case 11: {
            printAllFilials(globalFilials);
            cout << "Select Branch index: ";
            int bIdx; cin >> bIdx; bIdx--;
            int filialsCount = globalFilials.size();
            if (bIdx < 0 || bIdx >= filialsCount) { cout << "Invalid Branch index!\n"; break; }

            cout << "Enter specialty to search for: ";
            string specialty; cin >> ws; getline(cin, specialty);

            auto found = findDoctorBySpecialty(*globalFilials[bIdx], specialty);
            if (found) {
                cout << "-> Found: " << *found << "\n";
            }
            else {
                cout << "-> No doctor with specialty \"" << specialty
                    << "\" at \"" << globalFilials[bIdx]->getName() << "\".\n";
            }
            break;
        }

        case 12: {
            printAllFilials(globalFilials);
            cout << "Select Branch index: ";
            int bIdx; cin >> bIdx; bIdx--;
            int filialsCount = globalFilials.size();
            if (bIdx < 0 || bIdx >= filialsCount) { cout << "Invalid Branch index!\n"; break; }

            vector<shared_ptr<ClinicRecord>> all = globalFilials[bIdx]->getAllEntities();
            if (all.empty()) { cout << "(This branch has nothing assigned yet)\n"; break; }

            cout << "\n--- Polymorphic overview of \"" << globalFilials[bIdx]->getName() << "\" ---\n";
            cout << "(identical loop body for every entry — output differs by actual type)\n";
            for (const auto& entity : all) {
                cout << "[" << entity->getEntityType() << "] " << *entity
                    << " -> " << entity->classify() << "\n";
            }
            break;
        }

        case 0: cout << "Goodbye!\n"; break;
        default: cout << "Wrong option!\n"; break;
        }
    }
    return 0;
}