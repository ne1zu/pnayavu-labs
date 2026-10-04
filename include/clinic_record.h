#pragma once
#include <string>
#include <string_view>
#include <iostream>

class ClinicRecord {
protected:
    int id;
    std::string name;

public:
   
    ClinicRecord(std::string_view name, int& typeCounter);
    virtual ~ClinicRecord() = default;

    int getId() const;
    std::string getName() const;
    void setName(std::string_view nameValue);

    virtual std::string getEntityType() const = 0; 
    virtual void printInfo(std::ostream& os) const;  
    virtual std::string classify() const = 0;        
                                                       
    friend std::ostream& operator<<(std::ostream& os, const ClinicRecord& entity);
};
