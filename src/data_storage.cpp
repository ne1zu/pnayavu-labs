#include "data_storage.h"
#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include "exceptions/exceptions.h"

using namespace std;

namespace {
    const char FIELD_SEPARATOR = '|';
    const string FORMAT_HEADER = "CLINIC_STATE";
    const int FORMAT_VERSION = 1;
    const int MAX_ID = 1000000000;

    const string SERVICES_SECTION = "SERVICES";
    const string DOCTORS_SECTION = "DOCTORS";
    const string PATIENTS_SECTION = "PATIENTS";
    const string ADMINISTRATORS_SECTION = "ADMINISTRATORS";
    const string FILIALS_SECTION = "FILIALS";
    const string LINKS_SECTION = "LINKS";
    const string APPOINTMENTS_SECTION = "APPOINTMENTS";

    const string DOCTOR_CODE = "DOCTOR";
    const string SERVICE_CODE = "SERVICE";
    const string PATIENT_CODE = "PATIENT";
    const string ADMINISTRATOR_CODE = "ADMINISTRATOR";

    const size_t HEADER_FIELD_COUNT = 2;
    const size_t SECTION_FIELD_COUNT = 2;
    const size_t SERVICE_FIELD_COUNT = 4;
    const size_t DOCTOR_FIELD_COUNT = 4;
    const size_t PATIENT_FIELD_COUNT = 4;
    const size_t ADMINISTRATOR_FIELD_COUNT = 5;
    const size_t FILIAL_FIELD_COUNT = 7;
    const size_t LINK_FIELD_COUNT = 3;
    const size_t APPOINTMENT_FIELD_COUNT = 10;

    struct LoadContext {
        ifstream file;
        string filename;
        int lineNumber;
        ClinicData data;

        explicit LoadContext(const string& name) : file(name), filename(name), lineNumber(0) {}
    };

    void checkText(const string& value, const string& description) {
        if (value.find_first_of(string(1, FIELD_SEPARATOR) + "\r\n") != string::npos) {
            throw InvalidDataException(description, "must not contain '" + string(1, FIELD_SEPARATOR)
                + "' or a line break, got \"" + value + "\"");
        }
    }

    void checkTextFields(const ClinicData& data) {
        for (const auto& service : data.services) checkText(service->getName(), "service name");
        for (const auto& doctor : data.doctors) {
            checkText(doctor->getName(), "doctor name");
            checkText(doctor->getSpecialty(), "doctor specialty");
        }
        for (const auto& patient : data.patients) {
            checkText(patient->getName(), "patient name");
            checkText(patient->getPhone(), "patient phone");
        }
        for (const auto& admin : data.administrators) {
            checkText(admin->getName(), "administrator name");
            checkText(admin->getPosition(), "administrator position");
        }
        for (const auto& filial : data.filials) {
            checkText(filial->getName(), "branch name");
            checkText(filial->getAddress(), "branch address");
        }
    }

    int countLinks(const ClinicData& data) {
        int total = 0;
        for (const auto& filial : data.filials) {
            total += filial->getDoctorCount() + filial->getServiceCount()
                + filial->getPatientCount() + filial->getAdministratorCount();
        }
        return total;
    }

    int countAppointments(const ClinicData& data) {
        int total = 0;
        for (const auto& filial : data.filials) total += filial->getAppointmentCount();
        return total;
    }

    void writeServices(ostream& os, const ClinicData& data) {
        os << SERVICES_SECTION << FIELD_SEPARATOR << data.services.size() << '\n';
        for (const auto& service : data.services) {
            os << service->getId() << FIELD_SEPARATOR << service->getName() << FIELD_SEPARATOR
                << service->getPrice() << FIELD_SEPARATOR << service->getLength() << '\n';
        }
    }

    void writeDoctors(ostream& os, const ClinicData& data) {
        os << DOCTORS_SECTION << FIELD_SEPARATOR << data.doctors.size() << '\n';
        for (const auto& doctor : data.doctors) {
            os << doctor->getId() << FIELD_SEPARATOR << doctor->getName() << FIELD_SEPARATOR
                << doctor->getSpecialty() << FIELD_SEPARATOR << doctor->getExperience() << '\n';
        }
    }

    void writePatients(ostream& os, const ClinicData& data) {
        os << PATIENTS_SECTION << FIELD_SEPARATOR << data.patients.size() << '\n';
        for (const auto& patient : data.patients) {
            os << patient->getId() << FIELD_SEPARATOR << patient->getName() << FIELD_SEPARATOR
                << patient->getAge() << FIELD_SEPARATOR << patient->getPhone() << '\n';
        }
    }

    void writeAdministrators(ostream& os, const ClinicData& data) {
        os << ADMINISTRATORS_SECTION << FIELD_SEPARATOR << data.administrators.size() << '\n';
        for (const auto& admin : data.administrators) {
            os << admin->getId() << FIELD_SEPARATOR << admin->getName() << FIELD_SEPARATOR
                << admin->getPosition() << FIELD_SEPARATOR << admin->getExperience() << FIELD_SEPARATOR
                << admin->getPatientsPerDay() << '\n';
        }
    }

    void writeFilials(ostream& os, const ClinicData& data) {
        os << FILIALS_SECTION << FIELD_SEPARATOR << data.filials.size() << '\n';
        for (const auto& filial : data.filials) {
            os << filial->getName() << FIELD_SEPARATOR << filial->getAddress() << FIELD_SEPARATOR
                << filial->getDoctorCapacity() << FIELD_SEPARATOR << filial->getServiceCapacity() << FIELD_SEPARATOR
                << filial->getPatientCapacity() << FIELD_SEPARATOR << filial->getAdministratorCapacity() << FIELD_SEPARATOR
                << filial->getAppointmentCapacity() << '\n';
        }
    }

    void writeLinks(ostream& os, const ClinicData& data) {
        os << LINKS_SECTION << FIELD_SEPARATOR << countLinks(data) << '\n';
        for (const auto& filial : data.filials) {
            const string prefix = filial->getName() + FIELD_SEPARATOR;
            for (const auto& doctor : filial->getDoctors()) {
                os << prefix << DOCTOR_CODE << FIELD_SEPARATOR << doctor->getId() << '\n';
            }
            for (const auto& service : filial->getServices()) {
                os << prefix << SERVICE_CODE << FIELD_SEPARATOR << service->getId() << '\n';
            }
            for (const auto& patient : filial->getPatients()) {
                os << prefix << PATIENT_CODE << FIELD_SEPARATOR << patient->getId() << '\n';
            }
            for (const auto& admin : filial->getAdministrators()) {
                os << prefix << ADMINISTRATOR_CODE << FIELD_SEPARATOR << admin->getId() << '\n';
            }
        }
    }

    void writeAppointments(ostream& os, const ClinicData& data) {
        os << APPOINTMENTS_SECTION << FIELD_SEPARATOR << countAppointments(data) << '\n';
        for (const auto& filial : data.filials) {
            for (const auto& appointment : filial->getAppointments()) {
                os << appointment->getId() << FIELD_SEPARATOR << filial->getName() << FIELD_SEPARATOR
                    << appointment->getPatient()->getId() << FIELD_SEPARATOR
                    << appointment->getDoctor()->getId() << FIELD_SEPARATOR
                    << appointment->getService()->getId() << FIELD_SEPARATOR
                    << appointment->getDay() << FIELD_SEPARATOR << appointment->getMonth() << FIELD_SEPARATOR
                    << appointment->getYear() << FIELD_SEPARATOR << appointment->getHour() << FIELD_SEPARATOR
                    << appointment->getMinute() << '\n';
            }
        }
    }

    [[noreturn]] void fail(const LoadContext& ctx, const string& message) {
        throw FileFormatException(ctx.lineNumber, message);
    }

    string readLine(LoadContext& ctx) {
        string line;
        if (!getline(ctx.file, line)) {
            if (!ctx.file.eof()) throw FileIOException(ctx.filename, "reading");
            throw FileFormatException(ctx.lineNumber + 1, "unexpected end of file");
        }
        ctx.lineNumber++;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        return line;
    }

    vector<string> splitLine(const string& line) {
        vector<string> fields;
        size_t start = 0;
        size_t position = line.find(FIELD_SEPARATOR);
        while (position != string::npos) {
            fields.push_back(line.substr(start, position - start));
            start = position + 1;
            position = line.find(FIELD_SEPARATOR, start);
        }
        fields.push_back(line.substr(start));
        return fields;
    }

    vector<string> readFields(LoadContext& ctx, size_t expectedCount) {
        vector<string> fields = splitLine(readLine(ctx));
        if (fields.size() != expectedCount) {
            fail(ctx, "expected " + to_string(expectedCount) + " fields, got " + to_string(fields.size()));
        }
        return fields;
    }

    int parseInt(const string& text, const LoadContext& ctx, const string& field) {
        istringstream in(text);
        int value = 0;
        if (text.empty() || !(in >> value) || !in.eof()) {
            fail(ctx, "field '" + field + "' must be an integer, got \"" + text + "\"");
        }
        return value;
    }

    float parseFloat(const string& text, const LoadContext& ctx, const string& field) {
        istringstream in(text);
        float value = 0.0f;
        if (text.empty() || !(in >> value) || !in.eof() || !isfinite(value)) {
            fail(ctx, "field '" + field + "' must be a number, got \"" + text + "\"");
        }
        return value;
    }

    int parseId(const string& text, const LoadContext& ctx, const string& field) {
        int value = parseInt(text, ctx, field);
        if (value < 0 || value > MAX_ID) {
            fail(ctx, "field '" + field + "' must be between 0 and " + to_string(MAX_ID) + ", got " + text);
        }
        return value;
    }

    void requireText(const string& text, const LoadContext& ctx, const string& field) {
        if (text.empty()) fail(ctx, "field '" + field + "' must not be empty");
    }

    template <typename T>
    void requireNewId(const vector<shared_ptr<T>>& list, int id, const LoadContext& ctx, const string& kind) {
        for (const auto& item : list) {
            if (item->getId() == id) fail(ctx, "duplicate " + kind + " id " + to_string(id));
        }
    }

    template <typename T>
    shared_ptr<T> requireExisting(const vector<shared_ptr<T>>& list, int id, const LoadContext& ctx, const string& kind) {
        for (const auto& item : list) {
            if (item->getId() == id) return item;
        }
        fail(ctx, kind + " with id " + to_string(id) + " does not exist");
    }

    shared_ptr<Filial> findFilial(const LoadContext& ctx, const string& name) {
        for (const auto& filial : ctx.data.filials) {
            if (filial->getName() == name) return filial;
        }
        return nullptr;
    }

    shared_ptr<Filial> requireFilial(const LoadContext& ctx, const string& name) {
        shared_ptr<Filial> filial = findFilial(ctx, name);
        if (!filial) fail(ctx, "branch '" + name + "' is not described in the section " + FILIALS_SECTION);
        return filial;
    }

    void readHeader(LoadContext& ctx) {
        vector<string> fields = splitLine(readLine(ctx));
        if (fields[0] != FORMAT_HEADER) {
            fail(ctx, "this is not a clinic state file (the header '" + FORMAT_HEADER + "' was expected)");
        }
        if (fields.size() != HEADER_FIELD_COUNT) {
            fail(ctx, "expected " + to_string(HEADER_FIELD_COUNT) + " fields, got " + to_string(fields.size()));
        }
        int version = parseInt(fields[1], ctx, "format version");
        if (version != FORMAT_VERSION) {
            fail(ctx, "unsupported format version " + to_string(version) + " (supported: " + to_string(FORMAT_VERSION) + ")");
        }
    }

    int readSectionHeader(LoadContext& ctx, const string& sectionName) {
        vector<string> fields = readFields(ctx, SECTION_FIELD_COUNT);
        if (fields[0] != sectionName) {
            fail(ctx, "section '" + sectionName + "' was expected, got \"" + fields[0] + "\"");
        }
        int count = parseInt(fields[1], ctx, "number of records");
        if (count < 0) fail(ctx, "number of records must not be negative, got " + to_string(count));
        return count;
    }

    void readServices(LoadContext& ctx) {
        int count = readSectionHeader(ctx, SERVICES_SECTION);
        for (int i = 0; i < count; i++) {
            vector<string> f = readFields(ctx, SERVICE_FIELD_COUNT);
            int id = parseId(f[0], ctx, "service id");
            requireText(f[1], ctx, "service name");
            float price = parseFloat(f[2], ctx, "price");
            int length = parseInt(f[3], ctx, "duration");
            if (length <= 0) fail(ctx, "duration must be positive, got " + f[3]);
            requireNewId(ctx.data.services, id, ctx, "Service");
            ctx.data.services.push_back(make_shared<Service>(id, f[1], price, length));
        }
    }

    void readDoctors(LoadContext& ctx) {
        int count = readSectionHeader(ctx, DOCTORS_SECTION);
        for (int i = 0; i < count; i++) {
            vector<string> f = readFields(ctx, DOCTOR_FIELD_COUNT);
            int id = parseId(f[0], ctx, "doctor id");
            requireText(f[1], ctx, "doctor name");
            int experience = parseInt(f[3], ctx, "experience");
            requireNewId(ctx.data.doctors, id, ctx, "Doctor");
            ctx.data.doctors.push_back(make_shared<Doctor>(id, f[1], f[2], experience));
        }
    }

    void readPatients(LoadContext& ctx) {
        int count = readSectionHeader(ctx, PATIENTS_SECTION);
        for (int i = 0; i < count; i++) {
            vector<string> f = readFields(ctx, PATIENT_FIELD_COUNT);
            int id = parseId(f[0], ctx, "patient id");
            requireText(f[1], ctx, "patient name");
            int age = parseInt(f[2], ctx, "age");
            requireNewId(ctx.data.patients, id, ctx, "Patient");
            ctx.data.patients.push_back(make_shared<Patient>(id, f[1], age, f[3]));
        }
    }

    void readAdministrators(LoadContext& ctx) {
        int count = readSectionHeader(ctx, ADMINISTRATORS_SECTION);
        for (int i = 0; i < count; i++) {
            vector<string> f = readFields(ctx, ADMINISTRATOR_FIELD_COUNT);
            int id = parseId(f[0], ctx, "administrator id");
            requireText(f[1], ctx, "administrator name");
            int experience = parseInt(f[3], ctx, "experience");
            int perDay = parseInt(f[4], ctx, "patients per day");
            requireNewId(ctx.data.administrators, id, ctx, "Administrator");
            ctx.data.administrators.push_back(make_shared<Administrator>(id, f[1], f[2], experience, perDay));
        }
    }

    void readFilials(LoadContext& ctx) {
        int count = readSectionHeader(ctx, FILIALS_SECTION);
        for (int i = 0; i < count; i++) {
            vector<string> f = readFields(ctx, FILIAL_FIELD_COUNT);
            requireText(f[0], ctx, "branch name");
            if (findFilial(ctx, f[0])) fail(ctx, "duplicate branch name '" + f[0] + "'");
            int doctors = parseInt(f[2], ctx, "doctor capacity");
            int services = parseInt(f[3], ctx, "service capacity");
            int patients = parseInt(f[4], ctx, "patient capacity");
            int administrators = parseInt(f[5], ctx, "administrator capacity");
            int appointments = parseInt(f[6], ctx, "appointment capacity");
            ctx.data.filials.push_back(make_shared<Filial>(f[0], f[1], doctors, services, patients, administrators, appointments));
        }
    }

    void readLinks(LoadContext& ctx) {
        int count = readSectionHeader(ctx, LINKS_SECTION);
        for (int i = 0; i < count; i++) {
            vector<string> f = readFields(ctx, LINK_FIELD_COUNT);
            shared_ptr<Filial> filial = requireFilial(ctx, f[0]);
            int id = parseId(f[2], ctx, "object id");
            if (f[1] == DOCTOR_CODE) *filial += requireExisting(ctx.data.doctors, id, ctx, "Doctor");
            else if (f[1] == SERVICE_CODE) *filial += requireExisting(ctx.data.services, id, ctx, "Service");
            else if (f[1] == PATIENT_CODE) filial->restorePatient(requireExisting(ctx.data.patients, id, ctx, "Patient"));
            else if (f[1] == ADMINISTRATOR_CODE) *filial += requireExisting(ctx.data.administrators, id, ctx, "Administrator");
            else fail(ctx, "unknown object type \"" + f[1] + "\"");
        }
    }

    bool appointmentIdExists(const LoadContext& ctx, int id) {
        for (const auto& filial : ctx.data.filials) {
            for (const auto& appointment : filial->getAppointments()) {
                if (appointment->getId() == id) return true;
            }
        }
        return false;
    }

    void readAppointments(LoadContext& ctx) {
        int count = readSectionHeader(ctx, APPOINTMENTS_SECTION);
        for (int i = 0; i < count; i++) {
            vector<string> f = readFields(ctx, APPOINTMENT_FIELD_COUNT);
            int id = parseId(f[0], ctx, "appointment id");
            shared_ptr<Filial> filial = requireFilial(ctx, f[1]);
            shared_ptr<Patient> patient = requireExisting(ctx.data.patients, parseId(f[2], ctx, "patient id"), ctx, "Patient");
            shared_ptr<Doctor> doctor = requireExisting(ctx.data.doctors, parseId(f[3], ctx, "doctor id"), ctx, "Doctor");
            shared_ptr<Service> service = requireExisting(ctx.data.services, parseId(f[4], ctx, "service id"), ctx, "Service");
            int day = parseInt(f[5], ctx, "day");
            int month = parseInt(f[6], ctx, "month");
            int year = parseInt(f[7], ctx, "year");
            int hour = parseInt(f[8], ctx, "hour");
            int minute = parseInt(f[9], ctx, "minute");
            if (appointmentIdExists(ctx, id)) fail(ctx, "duplicate Appointment id " + to_string(id));
            *filial += make_shared<Appointment>(id, patient, doctor, service, f[1], day, month, year, hour, minute);
        }
    }

    void ensureNoTrailingData(LoadContext& ctx) {
        string line;
        while (getline(ctx.file, line)) {
            ctx.lineNumber++;
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (!line.empty()) fail(ctx, "extra data after the last record");
        }
        if (!ctx.file.eof()) throw FileIOException(ctx.filename, "reading");
    }
}

DataStorage::DataStorage(const string& filename) : filename(filename) {}

void DataStorage::save(const ClinicData& data) const {
    checkTextFields(data);

    ofstream file(filename);
    if (!file) throw FileOpenException(filename, "writing");

    file << setprecision(9);
    file << FORMAT_HEADER << FIELD_SEPARATOR << FORMAT_VERSION << '\n';
    writeServices(file, data);
    writeDoctors(file, data);
    writePatients(file, data);
    writeAdministrators(file, data);
    writeFilials(file, data);
    writeLinks(file, data);
    writeAppointments(file, data);

    file.close();
    if (!file) throw FileIOException(filename, "writing");
}

ClinicData DataStorage::load() const {
    LoadContext ctx(filename);
    if (!ctx.file) throw FileOpenException(filename, "reading");

    try {
        readHeader(ctx);
        readServices(ctx);
        readDoctors(ctx);
        readPatients(ctx);
        readAdministrators(ctx);
        readFilials(ctx);
        readLinks(ctx);
        readAppointments(ctx);
        ensureNoTrailingData(ctx);
    }
    catch (const FileFormatException&) {
        throw;
    }
    catch (const FileIOException&) {
        throw;
    }
    catch (const ClinicException& error) {
        throw FileFormatException(ctx.lineNumber, error.what());
    }

    return std::move(ctx.data);
}
