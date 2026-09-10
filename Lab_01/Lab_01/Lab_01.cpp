#include <iostream>
#include <string>
#include "service.h"
#include "doctor.h"
#include "filial.h"

void static printAllDoctors(Doctor** list, int count) {
    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << ". " << list[i]->getFio() << " (" << list[i]->getSpecialty() << ")\n";
    }
}

void static printAllServices(Service** list, int count) {
    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << ". " << list[i]->getName() << " (" << list[i]->getPrice() << " USD)\n";
    }
}

void static printAllFilials(Filial** list, int count) {
    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << ". " << list[i]->getName() << "\n";
    }
}

int main() {
    Service service1("Massage", 50.0f, 30);
    Service service2("Therapist_consultation", 25.5f, 15);
    Service service3("X-ray", 35.0f, 10);
    Service* allServices[] = { &service1, &service2, &service3 };
    const int serviceTotal = 3;

    Doctor doctor1("Ivanov_Ivan", "Therapist", 5);
    Doctor doctor2("Petrova_Anna", "Surgeon", 10);
    Doctor* allDoctors[] = { &doctor1, &doctor2 };
    const int doctorTotal = 2;

    Filial filial1("Central", "Main_st_10", 2, 3);
    Filial filial2("North", "Peace_st_25", 5, 5);
    Filial* allFilials[] = { &filial1, &filial2 };
    const int filialTotal = 2;

    filial1.addDoctor(&doctor1);
    filial1.addService(&service1);
    filial1.addService(&service2);

    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== CLINIC MANAGEMENT SYSTEM =====\n";
        std::cout << "1. View all Branches\n";
        std::cout << "2. View all Doctors\n";
        std::cout << "3. View all Services\n";
        std::cout << "4. Assign Doctor to a Branch\n";
        std::cout << "5. Assign Service to a Branch\n";
        std::cout << "6. Check Service availability\n";
        std::cout << "7. Edit Service Database\n";
        std::cout << "8. Edit Doctor Database\n";
        std::cout << "9. Edit Branch Database\n";
        std::cout << "0. Exit\n";
        std::cout << "Select an option: ";
        std::cin >> choice;
        std::cout << "\n";

        switch (choice) {
        case 1:
            for (int i = 0; i < filialTotal; i++) {
                allFilials[i]->printInfo();
                std::cout << "\n";
            }
            break;

        case 2:
            std::cout << "--- Doctors Database ---\n";
            printAllDoctors(allDoctors, doctorTotal);
            break;

        case 3:
            std::cout << "--- Services Database ---\n";
            printAllServices(allServices, serviceTotal);
            break;

        case 4: {
            std::cout << "Select Branch:\n";
            printAllFilials(allFilials, filialTotal);
            int f; std::cin >> f; f--;

            std::cout << "Select Doctor to assign:\n";
            printAllDoctors(allDoctors, doctorTotal);
            int d; std::cin >> d; d--;

            if (f >= 0 && f < filialTotal && d >= 0 && d < doctorTotal) {
                allFilials[f]->addDoctor(allDoctors[d]);
            }
            else {
                std::cout << "Invalid input!\n";
            }
            break;
        }

        case 5: {
            std::cout << "Select Branch:\n";
            printAllFilials(allFilials, filialTotal);
            int f; std::cin >> f; f--;

            std::cout << "Select Service to assign:\n";
            printAllServices(allServices, serviceTotal);
            int s; std::cin >> s; s--;

            if (f >= 0 && f < filialTotal && s >= 0 && s < serviceTotal) {
                allFilials[f]->addService(allServices[s]);
            }
            else {
                std::cout << "Invalid input!\n";
            }
            break;
        }

        case 6: {
            std::cout << "Select Branch:\n";
            printAllFilials(allFilials, filialTotal);
            int f; std::cin >> f; f--;

            std::cout << "Select Service to check:\n";
            printAllServices(allServices, serviceTotal);
            int s; std::cin >> s; s--;

            if (f >= 0 && f < filialTotal && s >= 0 && s < serviceTotal) {
                bool available = allFilials[f]->isServiceAvailable(allServices[s]->getId());
                std::cout << "Service \"" << allServices[s]->getName() << "\" in branch \""
                    << allFilials[f]->getName() << "\": "
                    << (available ? "available" : "unavailable") << "\n";
            }
            else {
                std::cout << "Invalid input!\n";
            }
            break;
        }

        case 7: {
            std::cout << "Select Service to edit:\n";
            printAllServices(allServices, serviceTotal);
            int s; std::cin >> s; s--;

            if (s >= 0 && s < serviceTotal) {
                int subChoice = -1;
                while (subChoice != 0) {
                    std::cout << "\n--- Editing Service: " << allServices[s]->getName() << " ---\n";
                    std::cout << "1. Edit Name\n2. Edit Price\n3. Edit Duration\n0. Back\nChoice: ";
                    std::cin >> subChoice;

                    switch (subChoice) {
                    case 1: {
                        std::string newName;
                        std::cout << "New name (no spaces): ";
                        std::cin >> newName;
                        allServices[s]->setName(newName);
                        std::cout << "Updated!\n"; break;
                    }
                    case 2: {
                        float p; std::cout << "New price: "; std::cin >> p;
                        allServices[s]->setPrice(p);
                        std::cout << "Updated!\n"; break;
                    }
                    case 3: {
                        int l; std::cout << "New duration (min): "; std::cin >> l;
                        allServices[s]->setLength(l);
                        std::cout << "Updated!\n"; break;
                    }
                    }
                }
            }
            else { std::cout << "Invalid input!\n"; }
            break;
        }

        case 8: {
            std::cout << "Select Doctor to edit:\n";
            printAllDoctors(allDoctors, doctorTotal);
            int d; std::cin >> d; d--;

            if (d >= 0 && d < doctorTotal) {
                int subChoice = -1;
                while (subChoice != 0) {
                    std::cout << "\n--- Editing Doctor: " << allDoctors[d]->getFio() << " ---\n";
                    std::cout << "1. Edit Name\n2. Edit Specialty\n3. Edit Experience\n0. Back\nChoice: ";
                    std::cin >> subChoice;

                    switch (subChoice) {
                    case 1: {
                        std::string newName;
                        std::cout << "New name (no spaces): ";
                        std::cin >> newName;
                        allDoctors[d]->setFio(newName);
                        std::cout << "Updated!\n"; break;
                    }
                    case 2: {
                        std::string newSpec;
                        std::cout << "New specialty (no spaces): ";
                        std::cin >> newSpec;
                        allDoctors[d]->setSpecialty(newSpec);
                        std::cout << "Updated!\n"; break;
                    }
                    case 3: {
                        int exp; std::cout << "New experience (years): "; std::cin >> exp;
                        allDoctors[d]->setExperience(exp);
                        std::cout << "Updated!\n"; break;
                    }
                    }
                }
            }
            else { std::cout << "Invalid input!\n"; }
            break;
        }

        case 9: {
            std::cout << "Select Branch to edit:\n";
            printAllFilials(allFilials, filialTotal);
            int f; std::cin >> f; f--;
            if (f >= 0 && f < filialTotal) {
                int subChoice = -1;
                while (subChoice != 0) {
                    std::cout << "\n--- Editing Branch: " << allFilials[f]->getName() << " ---\n";
                    std::cout << "1. Edit Name\n2. Edit Address\n0. Back\nChoice: ";
                    std::cin >> subChoice;

                    switch (subChoice) {
                    case 1: {
                        std::string newName;
                        std::cout << "New name (no spaces): ";
                        std::cin >> newName;
                        allFilials[f]->setName(newName);
                        std::cout << "Updated!\n"; break;
                    }
                    case 2: {
                        std::string newAddr;
                        std::cout << "New address (no spaces): ";
                        std::cin >> newAddr;
                        allFilials[f]->setAddress(newAddr);
                        std::cout << "Updated!\n"; break;
                    }
                    }
                }
            }
            else { std::cout << "Invalid input!\n"; }
            break;
        }

        case 0:
            std::cout << "Exiting...\n";
            break;

        default:
            std::cout << "Unknown command!\n";
            break;
        }
    }

    return 0;
}