#include "service.h"
#include <iostream>

int Service::counter = 0;

Service::Service(std::string_view name, float price, int length)
    : name(name), price(price), length(length)
{
    id = counter++;
}

int Service::getId() const { return id; }
std::string Service::getName() const { return name; }
float Service::getPrice() const { return price; }
int Service::getLength() const { return length; }

void Service::setName(std::string_view nameValue) { name = nameValue; }
void Service::setPrice(float priceValue) { price = priceValue; }
void Service::setLength(int lengthValue) { length = lengthValue; }

void Service::printInfo() const {
    std::cout << "Service #" << id << "\n"
        << "  Name: " << name << "\n"
        << "  Price: " << price << " USD\n"
        << "  Duration: " << length << " min\n";
}