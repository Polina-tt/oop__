#include <iostream>
#include <vector>

class ExperimentItem {
public:
    ExperimentItem() { std::cout << "Default Constructor\n"; }
    ~ExperimentItem() { std::cout << "Destructor\n"; }
    
    ExperimentItem(const ExperimentItem&) { std::cout << "Copy Constructor\n"; }
    ExperimentItem(ExperimentItem&&) noexcept { std::cout << "Move Constructor\n"; }
    
    ExperimentItem& operator=(const ExperimentItem&) {
        std::cout << "Copy Assignment\n";
        return *this;
    }
    
    ExperimentItem& operator=(ExperimentItem&&) noexcept {
        std::cout << "Move Assignment\n";
        return *this;
    }
};

void passByValue(ExperimentItem item) {}
void passByConstRef(const ExperimentItem& item) {}

int main() {
    std::cout << "--- 1. Створення об'єкта ---\n";
    ExperimentItem item1;

    std::cout << "\n--- 2. Створення копії ---\n";
    ExperimentItem item2 = item1;

    std::cout << "\n--- 3. Передавання за значенням ---\n";
    passByValue(item1);

    std::cout << "\n--- 4. Передавання за const-посиланням ---\n";
    passByConstRef(item1);

    std::cout << "\n--- 5. Додавання до вектора (std::vector) ---\n";
    std::vector<ExperimentItem> vec;
    vec.push_back(item1); 
    vec.push_back(std::move(item1)); 

    std::cout << "\n--- Кінець програми ---\n";
    return 0;
}
