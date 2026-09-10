#include <iostream>
#include <string>
#include "service.h"
#include "doctor.h"
#include "filial.h"

void printAllDoctors(Doctor** list, int count) {
    if (count == 0) { std::cout << "(Database is empty)\n"; return; }
    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << ". " << list[i]->getFio() << " (" << list[i]->getSpecialty() << ")\n";
    }
}

void printAllServices(Service** list, int count) {
    if (count == 0) { std::cout << "(Database is empty)\n"; return; }
    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << ". " << list[i]->getName() << " (" << list[i]->getPrice() << " USD)\n";
    }
}

void printAllFilials(Filial** list, int count) {
    if (count == 0) { std::cout << "(Database is empty)\n"; return; }
    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << ". " << list[i]->getName() << "\n";
    }
}

void handleEditService(Service** allServices, int serviceTotal) {
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
                std::string newName; std::cout << "New name (no spaces): "; std::cin >> newName;
                allServices[s]->setName(newName); std::cout << "Updated!\n"; break;
            }
            case 2: {
                float p; std::cout << "New price: "; std::cin >> p;
                allServices[s]->setPrice(p); std::cout << "Updated!\n"; break;
            }
            case 3: {
                int l; std::cout << "New duration (min): "; std::cin >> l;
                allServices[s]->setLength(l); std::cout << "Updated!\n"; break;
            }
            case 0: break;
            default: std::cout << "Invalid input!\n"; break; 
            }
        }
    }
    else { std::cout << "Invalid input!\n"; }
}

void handleEditDoctor(Doctor** allDoctors, int doctorTotal) {
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
                std::string newName; std::cout << "New name (no spaces): "; std::cin >> newName;
                allDoctors[d]->setFio(newName); std::cout << "Updated!\n"; break;
            }
            case 2: {
                std::string newSpec; std::cout << "New specialty (no spaces): "; std::cin >> newSpec;
                allDoctors[d]->setSpecialty(newSpec); std::cout << "Updated!\n"; break;
            }
            case 3: {
                int exp; std::cout << "New experience (years): "; std::cin >> exp;
                allDoctors[d]->setExperience(exp); std::cout << "Updated!\n"; break;
            }
            case 0: break;
            default: std::cout << "Invalid input!\n"; break; 
            }
        }
    }
    else { std::cout << "Invalid input!\n"; }
}

void handleEditBranch(Filial** allFilials, int filialTotal) {
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
                std::string newName; std::cout << "New name (no spaces): "; std::cin >> newName;
                allFilials[f]->setName(newName); std::cout << "Updated!\n"; break;
            }
            case 2: {
                std::string newAddr; std::cout << "New address (no spaces): "; std::cin >> newAddr;
                allFilials[f]->setAddress(newAddr); std::cout << "Updated!\n"; break;
            }
            case 0: break;
            default: std::cout << "Invalid input!\n"; break; 
            }
        }
    }
    else { std::cout << "Invalid input!\n"; }
}



int main() {
    const int MAX_CAPACITY = 20;

    Service* allServices[MAX_CAPACITY];
    Doctor* allDoctors[MAX_CAPACITY];
    Filial* allFilials[MAX_CAPACITY];

    int currentServiceCount = 0;
    int currentDoctorCount = 0;
    int currentFilialCount = 0;

    allServices[currentServiceCount++] = new Service("Massage", 50.0f, 30);
    allServices[currentServiceCount++] = new Service("Therapist_consultation", 25.5f, 15);

    allDoctors[currentDoctorCount++] = new Doctor("Ivanov_Ivan", "Therapist", 5);
    allDoctors[currentDoctorCount++] = new Doctor("Petrova_Anna", "Surgeon", 10);

    allFilials[currentFilialCount++] = new Filial("Central", "Main_st_10", 5, 5);
    allFilials[currentFilialCount++] = new Filial("North", "Peace_st_25", 5, 5);

    allFilials[0]->addDoctor(allDoctors[0]);
    allFilials[0]->addService(allServices[0]);

    int choice = -1;
    while (choice != 0) {
        std::cout << "\n===== CLINIC MANAGEMENT SYSTEM =====\n";
        std::cout << "1. View all Branches\n2. View all Doctors\n3. View all Services\n";
        std::cout << "4. Assign Doctor to a Branch\n5. Assign Service to a Branch\n";
        std::cout << "6. Check Service availability\n7. Edit Service\n8. Edit Doctor\n9. Edit Branch\n";
        std::cout << "0. Exit\n";
        std::cout << "Select an option: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            for (int i = 0; i < currentFilialCount; i++) {
                allFilials[i]->printInfo(); std::cout << "\n";
            }
            break;
        case 2: printAllDoctors(allDoctors, currentDoctorCount); break;
        case 3: printAllServices(allServices, currentServiceCount); break;

        case 4: {
            printAllFilials(allFilials, currentFilialCount);
            int f; std::cin >> f; f--;
            printAllDoctors(allDoctors, currentDoctorCount);
            int d; std::cin >> d; d--;
            if (f >= 0 && f < currentFilialCount && d >= 0 && d < currentDoctorCount)
                allFilials[f]->addDoctor(allDoctors[d]);
            break;
        }
        case 5: {
            printAllFilials(allFilials, currentFilialCount);
            int f; std::cin >> f; f--;
            printAllServices(allServices, currentServiceCount);
            int s; std::cin >> s; s--;
            if (f >= 0 && f < currentFilialCount && s >= 0 && s < currentServiceCount)
                allFilials[f]->addService(allServices[s]);
            break;
        }
        case 6: {
            printAllFilials(allFilials, currentFilialCount);
            int f; std::cin >> f; f--;
            printAllServices(allServices, currentServiceCount);
            int s; std::cin >> s; s--;
            if (f >= 0 && f < currentFilialCount && s >= 0 && s < currentServiceCount) {
                bool available = allFilials[f]->isServiceAvailable(allServices[s]->getId());
                std::cout << "Service is " << (available ? "available" : "unavailable") << "\n";
            }
            break;
        }

             
        case 7: handleEditService(allServices, currentServiceCount); break;
        case 8: handleEditDoctor(allDoctors, currentDoctorCount); break;
        case 9: handleEditBranch(allFilials, currentFilialCount); break;
        case 0: std::cout << "Goodbye!\n"; break;
        default: std::cout << "Wrong option!\n"; break; 
        }
    }

    for (int i = 0; i < currentServiceCount; i++) delete allServices[i];
    for (int i = 0; i < currentDoctorCount; i++) delete allDoctors[i];
    for (int i = 0; i < currentFilialCount; i++) delete allFilials[i];

    return 0;
}
