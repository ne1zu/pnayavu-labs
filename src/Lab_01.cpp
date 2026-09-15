#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "service.h"
#include "doctor.h"
#include "filial.h"

void printAllDoctors(const std::vector<std::shared_ptr<Doctor>>& doctorList) {
    if (doctorList.empty()) { 
        std::cout << "(Database is empty)\n"; 
        return; 
    }
    for (int index = 0; index < doctorList.size(); index++) {
        std::cout << index + 1 << ". " << doctorList[index]->getFio() 
                  << " (" << doctorList[index]->getSpecialty() << ", " 
                  << doctorList[index]->getExperience() << " Years)\n";
    }
}

void  printAllServices(const std::vector<std::shared_ptr<Service>>& serviceList) {
    if (serviceList.empty()) { 
        std::cout << "(Database is empty)\n"; 
        return; 
    }
    for (int index = 0; index <serviceList.size(); index++) {
        std::cout << index + 1 << ". " << serviceList[index]->getName() 
                  << " (" << serviceList[index]->getPrice() << " USD, " 
                  << serviceList[index]->getLength() << " min)\n";
    }
}

void  printAllFilials(const std::vector<std::shared_ptr<Filial>>& filialList) {
    if (filialList.empty()) { 
        std::cout << "(Database is empty)\n"; 
        return; 
    }
    for (int index = 0; index < filialList.size(); index++) {
        std::cout << index + 1 << ". " << filialList[index]->getName() << "\n";
    }
}
void  handleEditService(const std::vector<std::shared_ptr<Service>>& serviceList) {
    std::cout << "Select Service to edit:\n";
    printAllServices(serviceList);
    
    int selectedIndex; 
    std::cin >> selectedIndex; 
    selectedIndex--; 
    if (selectedIndex >= 0 && selectedIndex < serviceList.size()) {
        std::shared_ptr<Service> currentService = serviceList[selectedIndex];
        currentService->printInfo();
        
        int menuChoice = -1;
        while (menuChoice != 0) {
            std::cout << "\n1. Edit Name\n2. Edit Price\n3. Edit Duration\n0. Back\nChoice: ";
            std::cin >> menuChoice;

            switch (menuChoice) {
            case 1: {
                std::string newName; 
                std::cout << "New name (no spaces): "; 
                std::cin >> newName;
                currentService->setName(newName); 
                std::cout << "Updated!\n"; 
                break;
            }
            case 2: {
                float newPrice; 
                std::cout << "New price: "; 
                std::cin >> newPrice;
                currentService->setPrice(newPrice); 
                std::cout << "Updated!\n"; 
                break;
            }
            case 3: {
                int newDuration; 
                std::cout << "New duration (min): "; 
                std::cin >> newDuration;
                currentService->setLength(newDuration); 
                std::cout << "Updated!\n"; 
                break;
            }
            case 0: 
                break;
            default: 
                std::cout << "Invalid input!\n"; 
                break;
            }
            
            if (menuChoice >= 1 && menuChoice <= 3) {
                currentService->printInfo();
            }
        }
    }
    else { 
        std::cout << "Invalid input!\n"; 
    }
}

void  handleEditDoctor(const std::vector<std::shared_ptr<Doctor>>& doctorList) {
    std::cout << "Select Doctor to edit:\n";
    printAllDoctors(doctorList);
    
    int selectedIndex; 
    std::cin >> selectedIndex; 
    selectedIndex--;

    if (selectedIndex >= 0 && selectedIndex < doctorList.size()) {
        std::shared_ptr<Doctor> currentDoctor = doctorList[selectedIndex];
        currentDoctor->printInfo();
        
        int menuChoice = -1;
        while (menuChoice != 0) {
            std::cout << "\n1. Edit Name\n2. Edit Specialty\n3. Edit Experience\n0. Back\nChoice: ";
            std::cin >> menuChoice;

            switch (menuChoice) {
            case 1: {
                std::string newName; 
                std::cout << "New name (no spaces): "; 
                std::cin >> newName;
                currentDoctor->setFio(newName); 
                std::cout << "Updated!\n"; 
                break;
            }
            case 2: {
                std::string newSpecialty; 
                std::cout << "New specialty (no spaces): "; 
                std::cin >> newSpecialty;
                currentDoctor->setSpecialty(newSpecialty); 
                std::cout << "Updated!\n"; 
                break;
            }
            case 3: {
                int newExperience; 
                std::cout << "New experience (years): "; 
                std::cin >> newExperience;
                currentDoctor->setExperience(newExperience); 
                std::cout << "Updated!\n"; 
                break;
            }
            case 0: 
                break;
            default: 
                std::cout << "Invalid input!\n"; 
                break;
            }
            
            if (menuChoice >= 1 && menuChoice <= 3) {
                currentDoctor->printInfo();
            }
        }
    }
    else { 
        std::cout << "Invalid input!\n"; 
    }
}

void  handleEditBranch(const std::vector<std::shared_ptr<Filial>>& filialList) {
    std::cout << "Select Branch to edit:\n";
    printAllFilials(filialList);
    
    int selectedIndex; 
    std::cin >> selectedIndex; 
    selectedIndex--;

    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(filialList.size())) {
        std::shared_ptr<Filial> currentFilial = filialList[selectedIndex];
        currentFilial->printInfo();
        
        int menuChoice = -1;
        while (menuChoice != 0) {
            std::cout << "\n1. Edit Name\n2. Edit Address\n0. Back\nChoice: ";
            std::cin >> menuChoice;

            switch (menuChoice) {
            case 1: {
                std::string newName; 
                std::cout << "New name (no spaces): "; 
                std::cin >> newName;
                currentFilial->setName(newName); 
                std::cout << "Updated!\n"; 
                break;
            }
            case 2: {
                std::string newAddress; 
                std::cout << "New address (no spaces): "; 
                std::cin >> newAddress;
                currentFilial->setAddress(newAddress); 
                std::cout << "Updated!\n"; 
                break;
            }
            case 0: 
                break;
            default: 
                std::cout << "Invalid input!\n"; 
                break;
            }
            
            if (menuChoice >= 1 && menuChoice <= 2) {
                currentFilial->printInfo();
            }
        }
    }
    else { 
        std::cout << "Invalid input!\n"; 
    }
}

int main() {
    std::vector<std::shared_ptr<Service>> globalServices;
    std::vector<std::shared_ptr<Doctor>> globalDoctors;
    std::vector<std::shared_ptr<Filial>> globalFilials;

    globalServices.push_back(std::make_shared<Service>("Massage", 50.0f, 30));
    globalServices.push_back(std::make_shared<Service>("Therapist_consultation", 25.5f, 15));
    globalServices.push_back(std::make_shared<Service>("Xray", 35.0f, 10));
    globalServices.push_back(std::make_shared<Service>("Ultrasound", 40.0f, 20));
    globalServices.push_back(std::make_shared<Service>("Dental_checkup", 30.0f, 25));

    globalDoctors.push_back(std::make_shared<Doctor>("Ivanov_Ivan", "Therapist", 5));
    globalDoctors.push_back(std::make_shared<Doctor>("Petrova_Anna", "Surgeon", 10));
    globalDoctors.push_back(std::make_shared<Doctor>("Sidorov_Petr", "Masseur", 3));
    globalDoctors.push_back(std::make_shared<Doctor>("Kuznetsova_Olga", "Dentist", 8));

    globalFilials.push_back(std::make_shared<Filial>("Central", "Main_st_10", 2, 3));
    globalFilials.push_back(std::make_shared<Filial>("North", "Peace_st_25", 5, 5));

    globalFilials[0]->addDoctor(globalDoctors[0]);
    globalFilials[0]->addDoctor(globalDoctors[1]);
    globalFilials[0]->addService(globalServices[0]);
    globalFilials[0]->addService(globalServices[1]);

    globalFilials[1]->addDoctor(globalDoctors[2]);
    globalFilials[1]->addService(globalServices[2]);
    globalFilials[1]->addService(globalServices[3]);

    int mainChoice = -1;
    while (mainChoice != 0) {
        std::cout << "\n===== CLINIC MANAGEMENT SYSTEM =====\n";
        std::cout << "1. View all Branches\n2. View all Doctors\n3. View all Services\n";
        std::cout << "4. Assign Doctor to a Branch\n5. Assign Service to a Branch\n";
        std::cout << "6. Check Service availability\n7. Edit Service\n8. Edit Doctor\n9. Edit Branch\n";
        std::cout << "0. Exit\n";
        std::cout << "Select an option: ";
        std::cin >> mainChoice;

        switch (mainChoice) {
        case 1:
            for (const auto& currentFilial : globalFilials) {
                std::cout << "[" << currentFilial->getName() << "] " << currentFilial->getAddress()
                          << " -- doctors: " << currentFilial->getDoctorCount()
                          << ", services: " << currentFilial->getServiceCount() << "\n";
                currentFilial->printInfo(); 
                std::cout << "\n";
            }
            break;
        case 2: 
            std::cout << "Doctors:\n"; 
            printAllDoctors(globalDoctors); 
            break;
        case 3: 
            std::cout << "Services:\n"; 
            printAllServices(globalServices); 
            break;
        case 4: {
            printAllFilials(globalFilials);
            int selectedFilialIndex; 
            std::cin >> selectedFilialIndex; 
            selectedFilialIndex--;
            
            printAllDoctors(globalDoctors);
            int selectedDoctorIndex; 
            std::cin >> selectedDoctorIndex; 
            selectedDoctorIndex--;
            
            if (selectedFilialIndex >= 0 && selectedFilialIndex < globalFilials.size() && 
                selectedDoctorIndex >= 0 && selectedDoctorIndex < globalDoctors.size()) 
            {
                globalFilials[selectedFilialIndex]->addDoctor(globalDoctors[selectedDoctorIndex]);
            }
            break;
        }
        case 5: {
            printAllFilials(globalFilials);
            int selectedFilialIndex; 
            std::cin >> selectedFilialIndex; 
            selectedFilialIndex--;
            
            printAllServices(globalServices);
            int selectedServiceIndex; 
            std::cin >> selectedServiceIndex; 
            selectedServiceIndex--;
            
            if (selectedFilialIndex >= 0 && selectedFilialIndex < globalFilials.size() && 
                selectedServiceIndex >= 0 && selectedServiceIndex < globalServices.size()) 
            {
                globalFilials[selectedFilialIndex]->addService(globalServices[selectedServiceIndex]);
            }
            break;
        }
        case 6: {
            printAllFilials(globalFilials);
            int selectedFilialIndex; 
            std::cin >> selectedFilialIndex; 
            selectedFilialIndex--;
            
            printAllServices(globalServices);
            int selectedServiceIndex; 
            std::cin >> selectedServiceIndex; 
            selectedServiceIndex--;
            
            if (selectedFilialIndex >= 0 && selectedFilialIndex < globalFilials.size() && 
                selectedServiceIndex >= 0 && selectedServiceIndex < globalServices.size()) 
            {
                bool isAvailable = globalFilials[selectedFilialIndex]->isServiceAvailable(globalServices[selectedServiceIndex]->getId());
                std::cout << "Service is " << (isAvailable ? "available" : "unavailable") << "\n";
            }
            break;
        }
        case 7: 
            handleEditService(globalServices); 
            break;
        case 8: 
            handleEditDoctor(globalDoctors); 
            break;
        case 9: 
            handleEditBranch(globalFilials); 
            break;
        case 0: 
            std::cout << "Goodbye!\n"; 
            break;
        default: 
            std::cout << "Wrong option!\n"; 
            break;
        }
    }
    return 0;
}