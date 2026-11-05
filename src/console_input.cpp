#include "console_input.h"
#include <iostream>
#include <limits>

bool readInt(int& value) {
    if (std::cin >> value) return true;
    if (std::cin.eof()) return false;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
}

int readMenuChoice() {
    int choice;
    if (readInt(choice)) return choice;
    if (std::cin.eof()) return 0;
    std::cout << "[ERROR] Enter a number from the menu.\n";
    return -1;
}

void discardRestOfLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
