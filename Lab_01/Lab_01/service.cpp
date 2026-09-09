#include "service.h"
#include <iostream>
 int Service::counter=0;

Service::Service(const std::string& nameV, float priceV, int lengthV)
{
    id = counter;
    counter++;
    name = nameV;
    price = priceV;
    length = lengthV;
}
int Service::getId() const { return id; }
std::string Service::getName() const { return name; }
float Service::getPrice()const { return price; }
int Service::getLength()const { return length; }
void Service::setPrice(float priceValue) { price = priceValue; }
void Service::setLength(int lengthValue) { length = lengthValue; }
void Service::printInfo() const 
{
    std::cout << "Service #" << id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Price: " << price << "\n";
    std::cout << "Duration: " << length << " min\n";
}