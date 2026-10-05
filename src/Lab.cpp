#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "service.h"
#include "doctor.h"
#include "patient.h"
#include "administrator.h"
#include "filial.h"
#include "person.h"

using namespace std;

void printAllDoctors(const vector<shared_ptr<Doctor>>& list) {
    if (list.empty()) { cout << "Empty\n"; return; }
    for (size_t i = 0; i < list.size(); i++) cout << i + 1 << ". " << *list[i] << "\n";
}
void printAllServices(const vector<shared_ptr<Service>>& list) {
    if (list.empty()) { cout << "Empty\n"; return; }
    for (size_t i = 0; i < list.size(); i++) cout << i + 1 << ". " << *list[i] << "\n";
}
void printAllPatients(const vector<shared_ptr<Patient>>& list) {
    if (list.empty()) { cout << "Empty\n"; return; }
    for (size_t i = 0; i < list.size(); i++) cout << i + 1 << ". " << *list[i] << "\n";
}
void printAllAdmins(const vector<shared_ptr<Administrator>>& list) {
    if (list.empty()) { cout << "Empty\n"; return; }
    for (size_t i = 0; i < list.size(); i++) cout << i + 1 << ". " << *list[i] << "\n";
}
void printAllFilials(const vector<shared_ptr<Filial>>& list) {
    if (list.empty()) { cout << "Empty\n"; return; }
    for (size_t i = 0; i < list.size(); i++)
        cout << i + 1 << ". " << list[i]->getName() << " (" << list[i]->getAddress() << ")\n";
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
shared_ptr<Administrator> findExistingAdmin(const vector<shared_ptr<Administrator>>& list, const Administrator& candidate) {
    for (const auto& item : list) if (*item == candidate) return item; return nullptr;
}

void interactiveCompare(const vector<shared_ptr<Doctor>>& doctors,
    const vector<shared_ptr<Service>>& services,
    const vector<shared_ptr<Patient>>& patients,
    const vector<shared_ptr<Administrator>>& admins) {

    cout << "\n1. Compare Doctors\n";
    cout << "2. Compare Services\n";
    cout << "3. Compare Patients\n";
    cout << "4. Compare Administrators\n";
    cout << "0. Cancel\nChoice: ";

    int choice;
    cin >> choice;

    if (choice == 1) {
        int count = doctors.size();
        if (count < 2) return;
        printAllDoctors(doctors);
        int idx1, idx2;
        cin >> idx1 >> idx2; idx1--; idx2--;
        if (idx1 >= 0 && idx1 < count && idx2 >= 0 && idx2 < count) {
            auto a1 = doctors[idx1]; auto a2 = doctors[idx2];
            if (*a1 == *a2) cout << "Same\n";
            else if (*a1 > *a2) cout << a1->getName() << " is >\n";
            else if (*a1 < *a2) cout << a2->getName() << " is >\n";
            else cout << "Equal\n";
        }
    }
    else if (choice == 2) {
        int count = services.size();
        if (count < 2) return;
        printAllServices(services);
        int idx1, idx2;
        cin >> idx1 >> idx2; idx1--; idx2--;
        if (idx1 >= 0 && idx1 < count && idx2 >= 0 && idx2 < count) {
            auto a1 = services[idx1]; auto a2 = services[idx2];
            if (*a1 == *a2) cout << "Same\n";
            else if (*a1 > *a2) cout << a1->getName() << " is >\n";
            else if (*a1 < *a2) cout << a2->getName() << " is >\n";
            else cout << "Equal\n";
        }
    }
    else if (choice == 3) {
        int count = patients.size();
        if (count < 2) return;
        printAllPatients(patients);
        int idx1, idx2;
        cin >> idx1 >> idx2; idx1--; idx2--;
        if (idx1 >= 0 && idx1 < count && idx2 >= 0 && idx2 < count) {
            auto a1 = patients[idx1]; auto a2 = patients[idx2];
            if (*a1 == *a2) cout << "Same\n";
            else if (*a1 > *a2) cout << a1->getName() << " is >\n";
            else if (*a1 < *a2) cout << a2->getName() << " is >\n";
            else cout << "Equal\n";
        }
    }
    else if (choice == 4) {
        int count = admins.size();
        if (count < 2) return;
        printAllAdmins(admins);
        int idx1, idx2;
        cin >> idx1 >> idx2; idx1--; idx2--;
        if (idx1 >= 0 && idx1 < count && idx2 >= 0 && idx2 < count) {
            auto a1 = admins[idx1]; auto a2 = admins[idx2];
            if (*a1 == *a2) cout << "Same\n";
            else if (*a1 > *a2) cout << a1->getName() << " is >\n";
            else if (*a1 < *a2) cout << a2->getName() << " is >\n";
            else cout << "Equal\n";
        }
    }
}

int main() {
    vector<shared_ptr<Service>> globalServices;
    vector<shared_ptr<Doctor>> globalDoctors;
    vector<shared_ptr<Patient>> globalPatients;
    vector<shared_ptr<Administrator>> globalAdmins;
    vector<shared_ptr<Filial>> globalFilials;

    globalServices.push_back(make_shared<Service>("Massage", 50.0f, 30));
    globalDoctors.push_back(make_shared<Doctor>("Ivanov Ivan", "Therapist", 5));
    globalPatients.push_back(make_shared<Patient>("Smirnov Oleg", 34, "+375291112233"));
    globalAdmins.push_back(make_shared<Administrator>("Petrov Petr", "Head", 25));

    globalFilials.push_back(make_shared<Filial>("Central", "Main st 10"));

    *globalFilials[0] += globalDoctors[0];
    *globalFilials[0] += globalServices[0];
    *globalFilials[0] += globalPatients[0];
    *globalFilials[0] += globalAdmins[0];

    int mainChoice = -1;
    while (mainChoice != 0) {
        cout << "\n1. View Branches\n2. View Doctors\n3. View Services\n4. View Patients\n5. View Admins\n6. Create Object\n7. Assign Object\n8. Remove Object\n9. Compare\n10. Delete Object\n11. Check Service\n12. Find Admin\n13. Polymorphic Overview\n0. Exit\nChoice: ";
        cin >> mainChoice;

        switch (mainChoice) {
        case 1:
            for (const auto& f : globalFilials) cout << *f;
            break;
        case 2: printAllDoctors(globalDoctors); break;
        case 3: printAllServices(globalServices); break;
        case 4: printAllPatients(globalPatients); break;
        case 5: printAllAdmins(globalAdmins); break;

        case 6: {
            cout << "1. Patient 2. Doctor 3. Service 4. Admin\nChoice: ";
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
                if (!findExistingAdmin(globalAdmins, a)) globalAdmins.push_back(make_shared<Administrator>(a));
            }
            break;
        }
        case 7: {
            printAllFilials(globalFilials);
            int bIdx; cin >> bIdx; bIdx--;
            if (bIdx >= 0 && bIdx < globalFilials.size()) {
                cout << "1. Doctor 2. Service 3. Patient 4. Admin\nChoice: ";
                int sub; cin >> sub;
                if (sub == 1) {
                    printAllDoctors(globalDoctors); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < globalDoctors.size()) *globalFilials[bIdx] += globalDoctors[idx];
                }
                else if (sub == 2) {
                    printAllServices(globalServices); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < globalServices.size()) *globalFilials[bIdx] += globalServices[idx];
                }
                else if (sub == 3) {
                    printAllPatients(globalPatients); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < globalPatients.size()) *globalFilials[bIdx] += globalPatients[idx];
                }
                else if (sub == 4) {
                    printAllAdmins(globalAdmins); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < globalAdmins.size()) *globalFilials[bIdx] += globalAdmins[idx];
                }
            }
            break;
        }
        case 8: {
            printAllFilials(globalFilials);
            int bIdx; cin >> bIdx; bIdx--;
            if (bIdx >= 0 && bIdx < globalFilials.size()) {
                cout << "1. Doctor 2. Service 3. Patient 4. Admin\nChoice: ";
                int sub; cin >> sub;
                if (sub == 1) {
                    printAllDoctors(globalDoctors); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < globalDoctors.size()) *globalFilials[bIdx] -= globalDoctors[idx];
                }
                else if (sub == 2) {
                    printAllServices(globalServices); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < globalServices.size()) *globalFilials[bIdx] -= globalServices[idx];
                }
                else if (sub == 3) {
                    printAllPatients(globalPatients); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < globalPatients.size()) *globalFilials[bIdx] -= globalPatients[idx];
                }
                else if (sub == 4) {
                    printAllAdmins(globalAdmins); int idx; cin >> idx; idx--;
                    if (idx >= 0 && idx < globalAdmins.size()) *globalFilials[bIdx] -= globalAdmins[idx];
                }
            }
            break;
        }
        case 9:
            interactiveCompare(globalDoctors, globalServices, globalPatients, globalAdmins);
            break;

        case 10: {
            cout << "1. Doctor 2. Service 3. Patient 4. Admin\nChoice: ";
            int sub; cin >> sub;
            if (sub == 1) {
                printAllDoctors(globalDoctors); int idx; cin >> idx; idx--;
                if (idx >= 0 && idx < globalDoctors.size()) {
                    auto toDel = globalDoctors[idx];
                    for (auto& f : globalFilials) *f -= toDel;
                    globalDoctors.erase(globalDoctors.begin() + idx);
                }
            }
            else if (sub == 2) {
                printAllServices(globalServices); int idx; cin >> idx; idx--;
                if (idx >= 0 && idx < globalServices.size()) {
                    auto toDel = globalServices[idx];
                    for (auto& f : globalFilials) *f -= toDel;
                    globalServices.erase(globalServices.begin() + idx);
                }
            }
            else if (sub == 3) {
                printAllPatients(globalPatients); int idx; cin >> idx; idx--;
                if (idx >= 0 && idx < globalPatients.size()) {
                    auto toDel = globalPatients[idx];
                    for (auto& f : globalFilials) *f -= toDel;
                    globalPatients.erase(globalPatients.begin() + idx);
                }
            }
            else if (sub == 4) {
                printAllAdmins(globalAdmins); int idx; cin >> idx; idx--;
                if (idx >= 0 && idx < globalAdmins.size()) {
                    auto toDel = globalAdmins[idx];
                    for (auto& f : globalFilials) *f -= toDel;
                    globalAdmins.erase(globalAdmins.begin() + idx);
                }
            }
            break;
        }
        case 11: {
            printAllFilials(globalFilials); int bIdx; cin >> bIdx; bIdx--;
            if (bIdx >= 0 && bIdx < globalFilials.size()) {
                printAllServices(globalServices); int sIdx; cin >> sIdx; sIdx--;
                if (sIdx >= 0 && sIdx < globalServices.size()) {
                    if (isServiceAvailable(*globalFilials[bIdx], *globalServices[sIdx])) cout << "Yes\n";
                    else cout << "No\n";
                }
            }
            break;
        }
        case 12: {
            printAllFilials(globalFilials); int bIdx; cin >> bIdx; bIdx--;
            if (bIdx >= 0 && bIdx < globalFilials.size()) {
                string pos; cin >> ws; getline(cin, pos);
                auto found = findAdminByPosition(*globalFilials[bIdx], pos);
                if (found) cout << *found << "\n";
            }
            break;
        }
        case 13: {
            printAllFilials(globalFilials); int bIdx; cin >> bIdx; bIdx--;
            if (bIdx >= 0 && bIdx < globalFilials.size()) {
                vector<shared_ptr<Person>> people = globalFilials[bIdx]->getAllPeople();
                for (const auto& p : people) {
                    cout << "[" << p->getEntityType() << "] ";
                    p->printInfo(cout);
                    cout << " -> " << p->classify() << "\n";
                }
            }
            break;
        }
        case 0: break;
        }
    }
    return 0;
}