#include "equipment_record.hpp"
#include <iostream>

int main() {
    std::vector<EquipmentRecord> items = {
        {101, "Ноутбук Lenovo", "Ноутбук", true},
        {102, "Проєктор Epson", "Проєктор", false},
        {103, "МФУ HP", "Принтер", true},
        {104, "Графічний планшет Wacom", "Периферія", true},
        {105, "Світлодіодна панель", "Освітлення", false}
    };

    // 3. Вивести повний список
    printAllEquipment(items);

    // 4. Вивести кількість доступних одиниць
    std::cout << "Кількість доступних одиниць: " << countAvailableEquipment(items) << "\n\n";

    // 5. Пошук наявного ідентифікатора
    int searchIdExist = 101;
    std::cout << "Пошук обладнання з ID " << searchIdExist << ":\n";
    if (auto idx = findEquipmentIndexById(items, searchIdExist); idx.has_value()) {
        std::cout << "Обладнання знайдено: " << items[idx.value()].name << "\n\n";
    }

    // 6. Пошук відсутнього ідентифікатора
    int searchIdMissing = 999;
    std::cout << "Пошук обладнання з ID " << searchIdMissing << ":\n";
    if (auto idx = findEquipmentIndexById(items, searchIdMissing); idx.has_value()) {
        std::cout << "Обладнання знайдено: " << items[idx.value()].name << "\n\n";
    } else {
        std::cout << "Обладнання не знайдено\n\n";
    }

    // 7 & 8. Зміна доступності та повторний вивід статусу
    int targetId = 101;
    std::cout << "Зміна доступності обладнання з ID " << targetId << " на false:\n";
    setEquipmentAvailability(items, targetId, false);
    
    if (auto idx = findEquipmentIndexById(items, targetId); idx.has_value()) {
        std::cout << "Новий статус: " << (items[idx.value()].isAvailable ? "доступне" : "видане") << "\n";
    }

    return 0;
}