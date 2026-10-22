#pragma once
#include <string>
#include "clinic_exception.h"

class IndexOutOfRangeException : public ClinicException {
private:
    int index;
    int size;

    static std::string makeMessage(int index, int size) {
        std::string text = "Index " + std::to_string(index) + " is out of range";
        if (size <= 0) return text + ", the collection is empty.";
        if (size == 1) return text + ", the only valid index is 0.";
        return text + ", valid indexes are 0.." + std::to_string(size - 1) + ".";
    }

public:
    IndexOutOfRangeException(int index, int size)
        : ClinicException(makeMessage(index, size)), index(index), size(size) {}

    int getIndex() const { return index; }
    int getSize() const { return size; }
};
