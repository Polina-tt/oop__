#include "equipment_record.hpp"
#include <iostream>

void printEquipment(const EquipmentRecord& equipment) {
    std::cout << "ID: " << equipment.id << "\n"
              << "Назва: " << equipment.name << "\n"
              << "Категорія: " << equipment.category << "\n"
              << "Статус: " << (equipment.isAvailable ? "доступне" : "видане") << "\n\n";
}

void printAllEquipment(const std::vector<EquipmentRecord>& equipment) {
    std::cout << "Університетське обладнання\n"
              << "--------------------------\n";
    for (const auto& item : equipment) {
        printEquipment(item);
    }
}

std::size_t countAvailableEquipment(const std::vector<EquipmentRecord>& equipment) {
    std::size_t count = 0;
    for (const auto& item : equipment) {
        if (item.isAvailable) {
            ++count;
        }
    }
    return count;
}

std::optional<std::size_t> findEquipmentIndexById(
    const std::vector<EquipmentRecord>& equipment,
    int id
) {
    for (std::size_t i = 0; i < equipment.size(); ++i) {
        if (equipment[i].id == id) {
            return i;
        }
    }
    return std::nullopt;
}

bool setEquipmentAvailability(
    std::vector<EquipmentRecord>& equipment,
    int id,
    bool isAvailable
) {
    auto indexOpt = findEquipmentIndexById(equipment, id);
    if (indexOpt.has_value()) {
        equipment[indexOpt.value()].isAvailable = isAvailable;
        return true;
    }
    return false;
}