#pragma once

#include <iostream>
#include <string>

template <typename T>
const T& findMax(const T& a, const T& b) {
    return (b > a) ? b : a;
}

template <typename T>
class Box {
private:
    T item;

public:
    explicit Box(const T& value) : item(value) {}

    const T& getItem() const {
        return item;
    }

    void setItem(const T& newValue) {
        item = newValue;
    }
};

template <typename T, typename U>
class Pair {
private:
    T first;
    U second;

public:
    Pair(const T& firstValue, const U& secondValue)
        : first(firstValue), second(secondValue) {}

    void printPair() const {
        std::cout << "(" << first << ", " << second << ")\n";
    }
};