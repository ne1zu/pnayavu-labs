#include <iostream>
#include <string>
#include "service.h"
#include "doctor.h"
#include "filial.h"

void static printList(Doctor** list, int count) {
    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << ". " << list[i]->getFio()
            << " (" << list[i]->getSpecialty() << ")\n";
    }
}

void static printList(Service** list, int count) {
    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << ". " << list[i]->getName()
            << " (" << list[i]->getPrice() << " USD)\n";
    }
}

void static printFilialList(Filial** list, int count) {
    for (int i = 0; i < count; i++) {
        std::cout << i + 1 << ". " << list[i]->getName() << "\n";
    }
}

int static readInt(const std::string& prompt) {
    int value;
    std::cout << prompt;
    std::cin >> value;
    return value;
}

int main() {
    Service service1("Massage", 50.0f, 30);
    Service service2("Therapist consultation", 25.5f, 15);
    Service service3("Abdominal ultrasound", 40.0f, 20);
    Service service4("X-ray", 35.0f, 10);
    Service* allServices[] = { &service1, &service2, &service3, &service4 };
    const int serviceTotal = 4;

    Doctor doctor1("Ivanov Ivan", "Therapist", 5);
    Doctor doctor2("Petrova Anna", "Surgeon", 10);
    Doctor doctor3("Sidorov Petr", "Masseur", 3);
    Doctor* allDoctors[] = { &doctor1, &doctor2, &doctor3 };
    const int doctorTotal = 3;

    Filial filial1("Central", "Main st. 10", 2, 3);
    Filial filial2("North", "Peace st. 25", 5, 5);
    Filial* allFilials[] = { &filial1, &filial2 };
    const int filialTotal = 2;

    std::cout << "===== Classes Demonstration =====\n\n";

    std::cout << "--- All Doctors ---\n";
    for (int i = 0; i < doctorTotal; i++) allDoctors[i]->printInfo();

    std::cout << "\n--- All Services ---\n";
    for (int i = 0; i < serviceTotal; i++) allServices[i]->printInfo();

    std::cout << "\n--- Service Setters/Getters Demo ---\n";
    std::cout << "Before change: price=" << service1.getPrice()
        << ", duration=" << service1.getLength() << " min\n";
    service1.setPrice(65.0f);
    service1.setLength(45);
    std::cout << "After change: price=" << service1.getPrice()
        << ", duration=" << service1.getLength() << " min\n";

    std::cout << "\n--- Doctor Setters/Getters Demo ---\n";
    std::cout << "Before change: specialty=" << doctor3.getSpecialty()
        << ", experience=" << doctor3.getExperience() << " years\n";
    doctor3.setSpecialty("Physiotherapist");
    doctor3.setExperience(4);
    std::cout << "After change: specialty=" << doctor3.getSpecialty()
        << ", experience=" << doctor3.getExperience() << " years\n";

    std::cout << "\n--- Adding doctors to branches (limit check) ---\n";
    filial1.addDoctor(&doctor1);
    filial1.addDoctor(&doctor2);
    filial1.addDoctor(&doctor3);
    filial2.addDoctor(&doctor3);

    std::cout << "\n--- Adding services to branches ---\n";
    filial1.addService(&service1);
    filial1.addService(&service2);
    filial2.addService(&service3);
    filial2.addService(&service4);

    std::cout << "\n--- Checking service availability in branch ---\n";
    std::cout << "Massage available in \"Central\": "
        << (filial1.isServiceAvailable(service1.getId()) ? "yes" : "no") << "\n";
    std::cout << "X-ray available in \"Central\": "
        << (filial1.isServiceAvailable(service4.getId()) ? "yes" : "no") << "\n";

    std::cout << "\n--- Branch Getters Demo ---\n";
    std::cout << filial1.getName() << " is located at " << filial1.getAddress()
        << ", doctors: " << filial1.getDoctorCount()
        << ", services: " << filial1.getServiceCount() << "\n";

    std::cout << "\n--- Full branch info before copying ---\n";
    for (int i = 0; i < filialTotal; i++) allFilials[i]->printInfo();

    std::cout << "\n--- Copy Constructor Demo ---\n";
    Filial filial2Copy=filial2;
    filial2Copy.addDoctor(&doctor1);

    std::cout << "Original (\"North\") after changing the copy - should remain the same:\n";
    filial2.printInfo();
    std::cout << "\nCopy (\"North\", copy) - has more doctors:\n";
    filial2Copy.printInfo();

    std::cout << "\n--- Assignment Operator Demo ---\n";
    Filial filial1Assigned("Temporary", "unknown", 1, 1);
    filial1Assigned = filial1;
    std::cout << "Object after assigning content of \"" << filial1.getName() << "\":\n";
    filial1Assigned.printInfo();

    int choice;
    do {
        std::cout << "\n===== MENU =====\n";
        std::cout << "1. Show info about all branches\n";
        std::cout << "2. Show all doctors\n";
        std::cout << "3. Show all services\n";
        std::cout << "4. Add doctor to branch\n";
        std::cout << "5. Add service to branch\n";
        std::cout << "6. Check service availability in branch\n";
        std::cout << "0. Exit\n";
        choice = readInt("Choice: ");

        switch (choice) {
        case 1:
            for (int i = 0; i < filialTotal; i++) allFilials[i]->printInfo();
            break;
        case 2:
            printList(allDoctors, doctorTotal);
            break;
        case 3:
            printList(allServices, serviceTotal);
            break;
        case 4: {
            std::cout << "Branches:\n";
            printFilialList(allFilials, filialTotal);
            int f = readInt("Select branch: ") - 1;
            std::cout << "Doctors:\n";
            printList(allDoctors, doctorTotal);
            int d = readInt("Select doctor: ") - 1;
            allFilials[f]->addDoctor(allDoctors[d]);
            break;
        }
        case 5: {
            std::cout << "Branches:\n";
            printFilialList(allFilials, filialTotal);
            int f = readInt("Select branch: ") - 1;
            std::cout << "Services:\n";
            printList(allServices, serviceTotal);
            int s = readInt("Select service: ") - 1;
            allFilials[f]->addService(allServices[s]);
            break;
        }
        case 6: {
            std::cout << "Branches:\n";
            printFilialList(allFilials, filialTotal);
            int f = readInt("Select branch: ") - 1;
            std::cout << "Services:\n";
            printList(allServices, serviceTotal);
            int s = readInt("Select service: ") - 1;
            bool available = allFilials[f]->isServiceAvailable(allServices[s]->getId());
            std::cout << "Service \"" << allServices[s]->getName() << "\" in branch \""
                << allFilials[f]->getName() << "\": "
                << (available ? "available" : "unavailable") << "\n";
            break;
        }
        default:
            break;
        }
    } while (choice != 0);

    return 0;
}