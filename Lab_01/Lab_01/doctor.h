
#include <string>

class Doctor {
private:
    int id;
    std::string fio;
    std::string specialty;
    int experience;

    static int counter;

public:
    Doctor(const std::string& fio, const std::string& specialty, int experience);

    int getId() const;
    std::string getFio() const;
    std::string getSpecialty() const;
    int getExperience() const;

    void setSpecialty(const std::string& specialty);
    void setExperience(int experience);

    void printInfo() const;
};