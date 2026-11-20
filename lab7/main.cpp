#include <iostream>
#include <string>
#include "templates.hpp"

int main() {
    std::cout << "=== Завдання 1: findMax ===\n";
    int maxInt = findMax(10, 25);
    double maxDouble = findMax(3.14, 2.71);
    std::string s1 = "apple";
    std::string s2 = "banana";
    std::string maxStr = findMax(s1, s2);

    std::cout << "Max int: " << maxInt << "\n";
    std::cout << "Max double: " << maxDouble << "\n";
    std::cout << "Max string: " << maxStr << "\n\n";

    std::cout << "=== Завдання 2: Box ===\n";
    Box<int> intBox(42);
    std::cout << "intBox initial: " << intBox.getItem() << "\n";
    intBox.setItem(100);
    std::cout << "intBox updated: " << intBox.getItem() << "\n";

    Box<double> doubleBox(12.5);
    std::cout << "doubleBox initial: " << doubleBox.getItem() << "\n";
    doubleBox.setItem(99.9);
    std::cout << "doubleBox updated: " << doubleBox.getItem() << "\n";

    Box<std::string> strBox("Hello");
    std::cout << "strBox initial: " << strBox.getItem() << "\n";
    strBox.setItem("C++ Templates");
    std::cout << "strBox updated: " << strBox.getItem() << "\n\n";

    std::cout << "=== Завдання 3: Pair ===\n";
    Pair<int, std::string> p1(1, "Kyiv");
    Pair<double, char> p2(9.5, 'A');
    Pair<std::string, int> p3("Score", 150);

    std::cout << "Pair 1: ";
    p1.printPair();

    std::cout << "Pair 2: ";
    p2.printPair();

    std::cout << "Pair 3: ";
    p3.printPair();

    return 0;
}