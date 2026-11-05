#include "stl_menu.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include "console_input.h"
#include "exceptions/exceptions.h"
#include "stl_analyzer.h"

using namespace std;

namespace {

    string formatNumber(double value) {
        ostringstream out;
        out << fixed << setprecision(2) << value;
        return out.str();
    }

    string readLine(const string& prompt) {
        cout << prompt;
        string text;
        cin >> ws;
        getline(cin, text);
        return text;
    }

    template <typename T>
    shared_ptr<T> readObject() {
        T candidate;
        cin >> candidate;
        if (!cin) {
            discardRestOfLine();
            throw InvalidDataException("input", "a number was expected, the object was not created");
        }
        return make_shared<T>(candidate);
    }

    template <typename Container>
    void printList(const string& title, const Container& items) {
        cout << title << " (" << items.size() << "):\n";
        if (items.empty()) {
            cout << "  none\n";
            return;
        }
        for_each(items.begin(), items.end(), [](const typename Container::value_type& item) {
            cout << "  " << *item << " [" << item->classify() << "]\n";
        });
    }

    template <typename T>
    void printOne(const string& title, const shared_ptr<T>& item) {
        cout << title << ": ";
        if (item) cout << *item << " [" << item->classify() << "]\n";
        else cout << "none\n";
    }

    template <typename Groups>
    void printGroups(const string& title, const Groups& groups) {
        typedef typename Groups::mapped_type::value_type Pointer;
        cout << title << ":\n";
        if (groups.empty()) {
            cout << "  none\n";
            return;
        }
        for_each(groups.begin(), groups.end(), [](const typename Groups::value_type& group) {
            cout << "  " << group.first << " (" << group.second.size() << "):\n";
            for_each(group.second.begin(), group.second.end(), [](const Pointer& item) {
                cout << "    " << *item << "\n";
            });
        });
    }

    void printStatistics(const string& title, const Statistics& stats) {
        cout << "  " << title << ": count = " << stats.count << ", sum = " << formatNumber(stats.sum)
            << ", min = " << formatNumber(stats.min) << ", max = " << formatNumber(stats.max)
            << ", average = " << formatNumber(stats.average) << "\n";
    }

    void showSizes(const StlAnalyzer& analyzer) {
        cout << "  vector doctors: " << analyzer.getDoctors().size()
            << ", vector patients: " << analyzer.getPatients().size()
            << ", vector services: " << analyzer.getServices().size() << "\n";
        cout << "  list appointments: " << analyzer.getAppointments().size()
            << ", map specialties: " << analyzer.getDoctorsBySpecialty().size()
            << ", set unique patients: " << analyzer.getUniquePatientCount() << "\n";
    }

    void showContainers(const StlAnalyzer& analyzer) {
        cout << "\n--- Containers and why they were chosen ---\n";
        cout << "vector        doctors, patients, services: random access, compact, fast sort\n";
        cout << "list          appointments: cancel in the middle without moving the other elements\n";
        cout << "unordered_map doctor id -> doctor: search by the unique id in O(1) on average\n";
        cout << "map           specialty -> doctors: grouping, the keys stay sorted\n";
        cout << "set           ids of patients with appointments: every id is stored only once\n\n";
        printList("vector doctors", analyzer.getDoctors());
        printList("vector patients", analyzer.getPatients());
        printList("vector services", analyzer.getServices());
        printList("list appointments", analyzer.getAppointments());
        showSizes(analyzer);
    }

    void showFindById(const StlAnalyzer& analyzer, int id) {
        printOne("unordered_map::find, doctor with id " + to_string(id), analyzer.findDoctorById(id));
    }

    void showFindByName(const StlAnalyzer& analyzer, const string& name) {
        printOne("find_if, doctor named '" + name + "'", analyzer.findDoctorByName(name));
    }

    void showFindDoctors(const StlAnalyzer& analyzer, const string& specialty, int minExperience) {
        string shown = specialty.empty() ? "any" : specialty;
        printList("copy_if, doctors: specialty " + shown + " and experience >= " + to_string(minExperience),
            analyzer.findDoctors(specialty, minExperience));
    }

    void showFindPatients(const StlAnalyzer& analyzer, int minAge, int maxAge) {
        printList("copy_if, patients aged " + to_string(minAge) + ".." + to_string(maxAge),
            analyzer.findPatients(minAge, maxAge));
    }

    void showFindServices(const StlAnalyzer& analyzer, int minPrice, int maxPrice) {
        printList("copy_if, services with price " + to_string(minPrice) + ".." + to_string(maxPrice) + "$",
            analyzer.findServices(minPrice, maxPrice));
    }

    void showFindAppointments(const StlAnalyzer& analyzer, int doctorId, int month) {
        string doctorText = doctorId < 0 ? "any" : to_string(doctorId);
        string monthText = month == 0 ? "any" : to_string(month);
        printList("copy_if, appointments: doctor id " + doctorText + ", month " + monthText,
            analyzer.findAppointments(doctorId, month));
    }

    void showSortedDoctors(const StlAnalyzer& analyzer, bool byName, bool ascending) {
        string field = byName ? "name" : "experience";
        printList("sort, doctors by " + field + (ascending ? ", ascending" : ", descending"),
            analyzer.sortedDoctors(byName, ascending));
    }

    void showSortedPatients(const StlAnalyzer& analyzer, bool ascending) {
        printList(string("sort, patients by age") + (ascending ? ", ascending" : ", descending"),
            analyzer.sortedPatients(ascending));
    }

    void showSortedServices(const StlAnalyzer& analyzer, bool ascending) {
        printList(string("sort, services by price") + (ascending ? ", ascending" : ", descending"),
            analyzer.sortedServices(ascending));
    }

    void showSortedAppointments(const StlAnalyzer& analyzer, bool ascending) {
        printList(string("list::sort, appointments by time") + (ascending ? ", ascending" : ", descending"),
            analyzer.sortedAppointments(ascending));
    }

    void showExtremes(const StlAnalyzer& analyzer) {
        cout << "\n--- min_element / max_element ---\n";
        auto doctors = findExtremes(analyzer.getDoctors());
        printOne("least experienced doctor", doctors.first);
        printOne("most experienced doctor", doctors.second);
        auto patients = findExtremes(analyzer.getPatients());
        printOne("youngest patient", patients.first);
        printOne("oldest patient", patients.second);
        auto services = findExtremes(analyzer.getServices());
        printOne("cheapest service", services.first);
        printOne("most expensive service", services.second);
        auto appointments = findExtremes(analyzer.getAppointments());
        printOne("earliest appointment", appointments.first);
        printOne("latest appointment", appointments.second);
    }

    void showCounts(const StlAnalyzer& analyzer, int minExperience, int price, int doctorId) {
        cout << "\n--- count_if / count ---\n";
        cout << "count_if, doctors with experience >= " << minExperience << ": "
            << analyzer.countDoctors(minExperience) << "\n";
        cout << "count_if, adult patients (age >= 18): " << analyzer.countAdultPatients() << "\n";
        cout << "count_if, services cheaper than " << price << "$: " << analyzer.countServicesCheaperThan(price) << "\n";
        cout << "count_if, morning appointments: " << analyzer.countMorningAppointments() << "\n";
        cout << "count, appointments of the doctor with id " << doctorId << ": "
            << analyzer.countAppointmentsOfDoctor(doctorId) << "\n";
    }

    void showGroups(const StlAnalyzer& analyzer) {
        cout << "\n--- Grouping and classification ---\n";
        printGroups("map: doctors by specialty", analyzer.getDoctorsBySpecialty());
        printGroups("map: doctors by classify()", groupByClassification(analyzer.getDoctors()));
        printGroups("map: patients by classify()", groupByClassification(analyzer.getPatients()));
        printGroups("map: services by classify()", groupByClassification(analyzer.getServices()));
        printGroups("map: appointments by classify()", groupByClassification(analyzer.getAppointments()));
        map<string, int> load = analyzer.countAppointmentsByDoctor();
        cout << "map<string, int>: number of appointments per doctor:\n";
        if (load.empty()) cout << "  none\n";
        for_each(load.begin(), load.end(), [](const pair<const string, int>& entry) {
            cout << "  " << entry.first << ": " << entry.second << "\n";
        });
    }

    void showStatistics(const StlAnalyzer& analyzer) {
        cout << "\n--- Statistics (transform + accumulate + min_element + max_element) ---\n";
        printStatistics("doctor experience, years", analyzer.doctorExperienceStatistics());
        printStatistics("patient age, years", analyzer.patientAgeStatistics());
        printStatistics("service price, $", analyzer.servicePriceStatistics());
        cout << "  appointments in total: " << analyzer.getAppointments().size() << "\n";
        cout << "  unique patients with appointments (set): " << analyzer.getUniquePatientCount() << "\n";
        cout << "  total revenue (accumulate): " << formatNumber(analyzer.getTotalRevenue()) << "$\n";
        cout << "  total length (accumulate): " << analyzer.getTotalMinutes() << " min\n";
        pair<string, int> busiest = analyzer.findBusiestDoctor();
        if (busiest.second > 0) {
            cout << "  busiest doctor (max_element over the map): " << busiest.first
                << ", appointments: " << busiest.second << "\n";
        }
    }

    void showOtherAlgorithms(const StlAnalyzer& analyzer) {
        cout << "\n--- for_each, transform, copy and lambda captures ---\n";
        const vector<shared_ptr<Doctor>>& doctors = analyzer.getDoctors();

        vector<string> names;
        transform(doctors.begin(), doctors.end(), back_inserter(names),
            [](const shared_ptr<Doctor>& doctor) { return doctor->getName(); });
        cout << "transform + copy to ostream_iterator, names of doctors: ";
        copy(names.begin(), names.end(), ostream_iterator<string>(cout, "; "));
        cout << "\n";

        int threshold = 10;
        int visited = 0;
        auto isSenior = [threshold, &visited](const shared_ptr<Doctor>& doctor) {
            visited++;
            return doctor->getExperience() >= threshold;
        };
        threshold = 100;
        int seniors = static_cast<int>(count_if(doctors.begin(), doctors.end(), isSenior));
        cout << "lambda [threshold, &visited]: threshold was copied when the lambda was created (10), "
            << "the later change to 100 does not affect it\n";
        cout << "  senior doctors: " << seniors << ", elements visited (changed through the reference): "
            << visited << "\n";
    }

    void addDoctorByUser(StlAnalyzer& analyzer) {
        analyzer.addDoctor(readObject<Doctor>());
        cout << "[OK] Doctor added to vector, unordered_map and map.\n";
        showSizes(analyzer);
    }

    void removeDoctorByUser(StlAnalyzer& analyzer) {
        printList("vector doctors", analyzer.getDoctors());
        cout << "Enter doctor id: ";
        int id;
        if (!readInt(id)) return;
        analyzer.removeDoctor(id);
        cout << "[OK] Doctor removed from vector, unordered_map, map; his appointments removed from list.\n";
        showSizes(analyzer);
    }

    void addPatientByUser(StlAnalyzer& analyzer) {
        analyzer.addPatient(readObject<Patient>());
        cout << "[OK] Patient added to vector.\n";
        showSizes(analyzer);
    }

    void removePatientByUser(StlAnalyzer& analyzer) {
        printList("vector patients", analyzer.getPatients());
        cout << "Enter patient id: ";
        int id;
        if (!readInt(id)) return;
        analyzer.removePatient(id);
        cout << "[OK] Patient removed from vector; his appointments removed from list.\n";
        showSizes(analyzer);
    }

    void addServiceByUser(StlAnalyzer& analyzer) {
        analyzer.addService(readObject<Service>());
        cout << "[OK] Service added to vector.\n";
        showSizes(analyzer);
    }

    void removeServiceByUser(StlAnalyzer& analyzer) {
        printList("vector services", analyzer.getServices());
        cout << "Enter service id: ";
        int id;
        if (!readInt(id)) return;
        analyzer.removeService(id);
        cout << "[OK] Service removed from vector; appointments with it removed from list.\n";
        showSizes(analyzer);
    }

    void cancelAppointmentByUser(StlAnalyzer& analyzer) {
        printList("list appointments", analyzer.getAppointments());
        cout << "Enter appointment id: ";
        int id;
        if (!readInt(id)) return;
        analyzer.cancelAppointment(id);
        cout << "[OK] Appointment removed from list.\n";
        showSizes(analyzer);
    }

    void searchByUser(const StlAnalyzer& analyzer) {
        cout << "1. Doctor by id\n2. Doctor by name\n3. Doctors by specialty and experience\n"
            << "4. Patients by age range\n5. Services by price range\n6. Appointments by doctor and month\nChoice: ";
        int choice;
        if (!readInt(choice)) return;
        if (choice == 1) {
            cout << "Enter doctor id: ";
            int id;
            if (readInt(id)) showFindById(analyzer, id);
        }
        else if (choice == 2) {
            showFindByName(analyzer, readLine("Enter doctor name: "));
        }
        else if (choice == 3) {
            string specialty = readLine("Enter specialty ('-' for any): ");
            if (specialty == "-") specialty = "";
            cout << "Enter minimum experience: ";
            int minExperience;
            if (readInt(minExperience)) showFindDoctors(analyzer, specialty, minExperience);
        }
        else if (choice == 4) {
            cout << "Enter minimum and maximum age: ";
            int minAge, maxAge;
            if (readInt(minAge) && readInt(maxAge)) showFindPatients(analyzer, minAge, maxAge);
        }
        else if (choice == 5) {
            cout << "Enter minimum and maximum price: ";
            int minPrice, maxPrice;
            if (readInt(minPrice) && readInt(maxPrice)) showFindServices(analyzer, minPrice, maxPrice);
        }
        else if (choice == 6) {
            cout << "Enter doctor id (-1 for any) and month (0 for any): ";
            int doctorId, month;
            if (readInt(doctorId) && readInt(month)) showFindAppointments(analyzer, doctorId, month);
        }
        else {
            cout << "[ERROR] Unknown option.\n";
        }
    }

    void sortByUser(const StlAnalyzer& analyzer) {
        cout << "1. Doctors by experience\n2. Doctors by name\n3. Patients by age\n"
            << "4. Services by price\n5. Appointments by time\nChoice: ";
        int choice;
        if (!readInt(choice)) return;
        if (choice < 1 || choice > 5) {
            cout << "[ERROR] Unknown option.\n";
            return;
        }
        cout << "1. Ascending\n2. Descending\nChoice: ";
        int direction;
        if (!readInt(direction)) return;
        bool ascending = direction != 2;
        if (choice == 1) showSortedDoctors(analyzer, false, ascending);
        else if (choice == 2) showSortedDoctors(analyzer, true, ascending);
        else if (choice == 3) showSortedPatients(analyzer, ascending);
        else if (choice == 4) showSortedServices(analyzer, ascending);
        else showSortedAppointments(analyzer, ascending);
    }

    void countByUser(const StlAnalyzer& analyzer) {
        cout << "Enter minimum experience of a doctor, price limit of a service and doctor id: ";
        int minExperience, price, doctorId;
        if (readInt(minExperience) && readInt(price) && readInt(doctorId)) {
            showCounts(analyzer, minExperience, price, doctorId);
        }
    }

    void printOwnMenu() {
        cout << "\n=== STL: MY OWN TEST (working copy of the clinic data) ===\n"
            << " 1. Show containers\n 2. Add doctor\n 3. Remove doctor\n 4. Add patient\n 5. Remove patient\n"
            << " 6. Add service\n 7. Remove service\n 8. Cancel appointment\n 9. Search\n10. Sort\n"
            << "11. Min / Max\n12. Count by condition\n13. Group / classify\n14. Statistics\n 0. Back\nChoice: ";
    }

    void executeOwnChoice(StlAnalyzer& analyzer, int choice) {
        switch (choice) {
        case 1: showContainers(analyzer); break;
        case 2: addDoctorByUser(analyzer); break;
        case 3: removeDoctorByUser(analyzer); break;
        case 4: addPatientByUser(analyzer); break;
        case 5: removePatientByUser(analyzer); break;
        case 6: addServiceByUser(analyzer); break;
        case 7: removeServiceByUser(analyzer); break;
        case 8: cancelAppointmentByUser(analyzer); break;
        case 9: searchByUser(analyzer); break;
        case 10: sortByUser(analyzer); break;
        case 11: showExtremes(analyzer); break;
        case 12: countByUser(analyzer); break;
        case 13: showGroups(analyzer); break;
        case 14: showStatistics(analyzer); break;
        case 0:
        case -1:
            break;
        default:
            cout << "[ERROR] Unknown option.\n";
            break;
        }
    }

    void runOwnTest(const ClinicData& data, const Logger& logger) {
        StlAnalyzer analyzer(data);
        logger.log("STL_TEST", "Own test started");
        cout << "\nThe data of the clinic is copied into STL containers. "
            << "Changes made here do not touch the clinic itself.\n";
        int choice = -1;
        while (choice != 0) {
            printOwnMenu();
            choice = readMenuChoice();
            try {
                executeOwnChoice(analyzer, choice);
            }
            catch (const ClinicException& error) {
                cout << "[ERROR] " << error.what() << "\n";
                logger.log("ERROR", string("[STL] ") + error.what());
            }
        }
    }

    void book(Filial& filial, const shared_ptr<Patient>& patient, const shared_ptr<Doctor>& doctor,
        const shared_ptr<Service>& service, int day, int month, int hour, int minute) {
        filial += make_shared<Appointment>(patient, doctor, service, filial.getName(), day, month, 2026, hour, minute);
    }

    void fillStlTestData(ClinicData& data) {
        data.services.push_back(make_shared<Service>("Consultation", 20.0f, 20));
        data.services.push_back(make_shared<Service>("Checkup", 35.0f, 30));
        data.services.push_back(make_shared<Service>("Ultrasound", 60.0f, 30));
        data.services.push_back(make_shared<Service>("MRI Scan", 150.0f, 45));
        data.services.push_back(make_shared<Service>("Dental Cleaning", 45.0f, 40));

        data.doctors.push_back(make_shared<Doctor>("Ivanov Ivan", "Therapist", 3));
        data.doctors.push_back(make_shared<Doctor>("Petrov Pavel", "Therapist", 7));
        data.doctors.push_back(make_shared<Doctor>("Sidorov Oleg", "Therapist", 15));
        data.doctors.push_back(make_shared<Doctor>("Kozlov Anton", "Surgeon", 4));
        data.doctors.push_back(make_shared<Doctor>("Morozov Dmitry", "Surgeon", 12));
        data.doctors.push_back(make_shared<Doctor>("Volkova Anna", "Pediatrician", 2));
        data.doctors.push_back(make_shared<Doctor>("Sokolova Elena", "Pediatrician", 9));
        data.doctors.push_back(make_shared<Doctor>("Lebedev Igor", "Cardiologist", 6));
        data.doctors.push_back(make_shared<Doctor>("Orlov Viktor", "Cardiologist", 18));
        data.doctors.push_back(make_shared<Doctor>("Zaytsev Maxim", "Dentist", 10));

        data.patients.push_back(make_shared<Patient>("Smirnov Oleg", 34, "+375291000001"));
        data.patients.push_back(make_shared<Patient>("Potter Harry", 12, "+375291000002"));
        data.patients.push_back(make_shared<Patient>("Wayne Bruce", 45, "+375291000003"));
        data.patients.push_back(make_shared<Patient>("Granger Hermione", 17, "+375291000004"));
        data.patients.push_back(make_shared<Patient>("Stark Tony", 52, "+375291000005"));
        data.patients.push_back(make_shared<Patient>("Parker Peter", 5, "+375291000006"));
        data.patients.push_back(make_shared<Patient>("Banner Bruce", 41, "+375291000007"));
        data.patients.push_back(make_shared<Patient>("Romanoff Natasha", 28, "+375291000008"));
        data.patients.push_back(make_shared<Patient>("Rogers Steve", 73, "+375291000009"));
        data.patients.push_back(make_shared<Patient>("Lang Scott", 18, "+375291000010"));

        data.filials.push_back(make_shared<Filial>("Central Clinic", "Main st 10"));
        data.filials.push_back(make_shared<Filial>("North Branch", "Cold st 1"));
        Filial& central = *data.filials[0];
        Filial& north = *data.filials[1];

        for (int i = 0; i < 5; i++) central += data.doctors[i];
        for (int i = 5; i < 10; i++) north += data.doctors[i];
        for (int i = 0; i < 5; i++) {
            central += data.services[i];
            north += data.services[i];
        }
        for (int i = 0; i < 5; i++) central += data.patients[i];
        for (int i = 5; i < 10; i++) north += data.patients[i];

        const vector<shared_ptr<Doctor>>& d = data.doctors;
        const vector<shared_ptr<Patient>>& p = data.patients;
        const vector<shared_ptr<Service>>& s = data.services;
        book(central, p[0], d[0], s[0], 3, 11, 8, 30);
        book(central, p[1], d[0], s[1], 3, 11, 10, 15);
        book(central, p[2], d[1], s[2], 4, 11, 13, 0);
        book(central, p[0], d[2], s[3], 5, 12, 15, 30);
        book(central, p[3], d[0], s[0], 6, 12, 9, 0);
        book(central, p[1], d[3], s[4], 7, 12, 18, 0);
        book(north, p[5], d[5], s[0], 3, 11, 9, 0);
        book(north, p[6], d[6], s[1], 4, 11, 11, 30);
        book(north, p[5], d[7], s[2], 5, 11, 14, 0);
        book(north, p[7], d[8], s[3], 6, 12, 16, 0);
        book(north, p[8], d[9], s[4], 7, 12, 8, 45);
        book(north, p[6], d[8], s[1], 8, 12, 12, 30);
    }

    void demoAddAndRemove(const StlAnalyzer& original) {
        cout << "\n=== STEP 1. ADD AND REMOVE (a copy of the containers is used, the other steps are not affected) ===\n";
        StlAnalyzer work = original;
        showSizes(work);

        cout << "\npush_back + insert + operator[]: add the doctor 'Demo Added' (Neurologist, 8 years)\n";
        shared_ptr<Doctor> added = make_shared<Doctor>("Demo Added", "Neurologist", 8);
        work.addDoctor(added);
        showSizes(work);
        printOne("unordered_map::find finds him at once by id", work.findDoctorById(added->getId()));

        cout << "\nThe same doctor once more:\n";
        try {
            work.addDoctor(make_shared<Doctor>("Demo Added", "Neurologist", 8));
        }
        catch (const ClinicException& error) {
            cout << "[caught] " << error.what() << "\n";
        }

        cout << "\nerase + remove: remove 'Demo Added' from the vector, the map and the unordered_map\n";
        work.removeDoctor(added->getId());
        showSizes(work);

        shared_ptr<Doctor> busy = work.findDoctorByName("Ivanov Ivan");
        cout << "\nremove 'Ivanov Ivan' who has " << work.countAppointmentsOfDoctor(busy->getId())
            << " appointments: list::remove_if cancels them too\n";
        work.removeDoctor(busy->getId());
        showSizes(work);

        cout << "\nadd the patient 'Demo Patient' and remove 'Smirnov Oleg' (erase-remove idiom):\n";
        work.addPatient(make_shared<Patient>("Demo Patient", 29, "+375290009999"));
        shared_ptr<Patient> patient = work.getPatients()[0];
        work.removePatient(patient->getId());
        showSizes(work);

        cout << "\nadd the service 'Demo Service' and remove 'Consultation' (its appointments are removed too):\n";
        work.addService(make_shared<Service>("Demo Service", 99.0f, 25));
        shared_ptr<Service> consultation = work.getServices()[0];
        work.removeService(consultation->getId());
        showSizes(work);

        cout << "\nlist::remove_if: cancel the first appointment of the list\n";
        work.cancelAppointment(work.getAppointments().front()->getId());
        showSizes(work);

        cout << "\nRemove a doctor that does not exist:\n";
        try {
            work.removeDoctor(-1);
        }
        catch (const ClinicException& error) {
            cout << "[caught] " << error.what() << "\n";
        }
    }

    void demoSearch(const StlAnalyzer& analyzer) {
        cout << "\n=== STEP 2. SEARCH BY ONE AND SEVERAL CRITERIA ===\n";
        shared_ptr<Doctor> cardiologist = analyzer.findDoctorByName("Orlov Viktor");
        showFindById(analyzer, cardiologist->getId());
        showFindById(analyzer, -5);
        showFindByName(analyzer, "Orlov Viktor");
        showFindByName(analyzer, "Nobody Here");
        showFindDoctors(analyzer, "Therapist", 0);
        showFindDoctors(analyzer, "Therapist", 7);
        showFindDoctors(analyzer, "", 12);
        showFindPatients(analyzer, 0, 17);
        showFindPatients(analyzer, 30, 60);
        showFindServices(analyzer, 30, 100);
        showFindAppointments(analyzer, -1, 11);
        showFindAppointments(analyzer, analyzer.findDoctorByName("Ivanov Ivan")->getId(), 12);
    }

    void demoSort(const StlAnalyzer& analyzer) {
        cout << "\n=== STEP 3. SORTING ===\n";
        showSortedDoctors(analyzer, false, false);
        showSortedDoctors(analyzer, true, true);
        showSortedPatients(analyzer, true);
        showSortedServices(analyzer, false);
        showSortedAppointments(analyzer, true);
    }

    void printDemoMenu() {
        cout << "\n=== STL: READY TEST (test data, the program works by itself) ===\n"
            << "1. Add and remove\n2. Search\n3. Sort\n4. Min / Max\n5. Count by condition\n"
            << "6. Group / classify\n7. Statistics\n8. for_each, copy, lambda captures\n"
            << "9. Show containers\n10. Run everything\n0. Back\nChoice: ";
    }

    void executeDemoChoice(const StlAnalyzer& analyzer, int choice) {
        shared_ptr<Doctor> ivanov = analyzer.findDoctorByName("Ivanov Ivan");
        switch (choice) {
        case 1: demoAddAndRemove(analyzer); break;
        case 2: demoSearch(analyzer); break;
        case 3: demoSort(analyzer); break;
        case 4: showExtremes(analyzer); break;
        case 5: showCounts(analyzer, 10, 40, ivanov->getId()); break;
        case 6: showGroups(analyzer); break;
        case 7: showStatistics(analyzer); break;
        case 8: showOtherAlgorithms(analyzer); break;
        case 9: showContainers(analyzer); break;
        case 10:
            executeDemoChoice(analyzer, 9);
            for (int step = 1; step <= 8; step++) executeDemoChoice(analyzer, step);
            break;
        case 0:
        case -1:
            break;
        default:
            cout << "[ERROR] Unknown option.\n";
            break;
        }
    }

    void runAutoDemo(const Logger& logger) {
        logger.log("STL_DEMO", "Ready test started");
        ClinicData testData;
        fillStlTestData(testData);
        StlAnalyzer analyzer(testData);
        cout << "\nTest data: 10 doctors, 10 patients, 5 services, 2 branches, 12 appointments.\n";
        int choice = -1;
        while (choice != 0) {
            printDemoMenu();
            choice = readMenuChoice();
            executeDemoChoice(analyzer, choice);
        }
    }

}

void runStlMenu(const ClinicData& data, const Logger& logger) {
    int choice = -1;
    while (choice != 0) {
        cout << "\n=== STL MENU ===\n"
            << "1. My own test (data of the clinic, I choose the operations)\n"
            << "2. Ready test on test data (I choose an item, the program does the rest)\n"
            << "0. Back\nChoice: ";
        choice = readMenuChoice();
        if (choice == 1) runOwnTest(data, logger);
        else if (choice == 2) runAutoDemo(logger);
        else if (choice != 0 && choice != -1) cout << "[ERROR] Unknown option.\n";
    }
}
