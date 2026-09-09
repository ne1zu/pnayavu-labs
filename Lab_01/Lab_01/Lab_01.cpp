#include "service.h"
#include <iostream>
using namespace std;

int main()
{
    setlocale(LC_ALL, "Rus");
    Service s1("Массаж", 50.0f, 30);
    Service s2("Консультация терапевта", 25.5f, 15);
    Service s3("УЗИ брюшной полости", 40.0f, 20);

    std::cout << "=== Начальное состояние ===\n";
    s1.printInfo();
    s2.printInfo();
    s3.printInfo();

    std::cout << "\n=== Проверка автоинкремента id ===\n";
    std::cout << "s1.id = " << s1.getId() << "\n";
    std::cout << "s2.id = " << s2.getId() << "\n";
    std::cout << "s3.id = " << s3.getId() << "\n";

    std::cout << "\n=== Проверка сеттеров ===\n";
    s1.setPrice(65.0f);
    s1.setLength(45);
    s1.printInfo();

    return 0;
}


