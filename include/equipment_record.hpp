#pragma once

#include <string>
#include <vector>
#include <optional>

struct EquipmentRecord {
    int id;
    std::string name;
    std::string category;
    bool isAvailable;
};

void printEquipment(const EquipmentRecord& equipment);

void printAllEquipment(const std::vector<EquipmentRecord>& equipment);

std::size_t countAvailableEquipment(const std::vector<EquipmentRecord>& equipment);

std::optional<std::size_t> findEquipmentIndexById(
    const std::vector<EquipmentRecord>& equipment,
    int id
);

bool setEquipmentAvailability(
    std::vector<EquipmentRecord>& equipment,
    int id,
    bool isAvailable
);