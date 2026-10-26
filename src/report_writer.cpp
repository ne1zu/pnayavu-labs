#include "report_writer.h"
#include <fstream>
#include "exceptions/exceptions.h"
#include "timestamp.h"

namespace {
    template <typename T>
    void writeList(std::ostream& os, const std::string& title, const Collection<T>& items, int capacity) {
        os << "  " << title << " (" << items.getCount() << "/" << capacity << "):\n";
        if (items.isEmpty()) {
            os << "    none\n";
            return;
        }
        int number = 1;
        for (const auto& item : items) {
            os << "    " << number << ". " << *item << " [" << item->classify() << "]\n";
            number++;
        }
    }

    int countAppointments(const ClinicData& data) {
        int total = 0;
        for (const auto& filial : data.filials) total += filial->getAppointmentCount();
        return total;
    }

    void writeSummary(std::ostream& os, const ClinicData& data) {
        os << "=== SUMMARY ===\n";
        os << "Branches: " << data.filials.size() << "\n";
        os << "Doctors: " << data.doctors.size() << "\n";
        os << "Services: " << data.services.size() << "\n";
        os << "Patients: " << data.patients.size() << "\n";
        os << "Administrators: " << data.administrators.size() << "\n";
        os << "Appointments: " << countAppointments(data) << "\n\n";
    }

    void writeBranches(std::ostream& os, const ClinicData& data) {
        os << "=== BRANCHES ===\n";
        if (data.filials.empty()) os << "none\n";
        for (const auto& filial : data.filials) {
            os << "Branch: " << filial->getName() << " (" << filial->getAddress() << ")\n";
            writeList(os, "Doctors", filial->getDoctors(), filial->getDoctorCapacity());
            writeList(os, "Services", filial->getServices(), filial->getServiceCapacity());
            writeList(os, "Patients", filial->getPatients(), filial->getPatientCapacity());
            writeList(os, "Administrators", filial->getAdministrators(), filial->getAdministratorCapacity());
            writeList(os, "Appointments", filial->getAppointments(), filial->getAppointmentCapacity());
            os << "\n";
        }
    }
}

ReportWriter::ReportWriter(const std::string& filename) : filename(filename) {}

void ReportWriter::write(const ClinicData& data) const {
    std::ofstream file(filename);
    if (!file) throw FileOpenException(filename, "writing");

    file << "CLINIC REPORT\n";
    file << "Generated: " << currentTimestamp() << "\n\n";
    writeSummary(file, data);
    writeBranches(file, data);

    file.close();
    if (!file) throw FileIOException(filename, "writing");
}
