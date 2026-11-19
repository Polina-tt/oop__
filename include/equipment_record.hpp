#pragma once

#include <string>
#include <vector>
#include <optional>
#include <stdexcept>
#include <utility>

enum class AccessCategory {
General,
Laboratory,
HighSecurity
};

class Equipment {
private:
int id_;
std::string name_;
AccessCategory category_;
bool restricted_;
bool available_;

public:
Equipment(
    int id,
    std::string name,
    AccessCategory category = AccessCategory::General,
    bool restricted = false,
    bool available = true
)
    : id_(id),
      name_(std::move(name)),
      category_(category),
      restricted_(restricted),
      available_(available) {
    if (id_ <= 0) {
        throw std::invalid_argument("Equipment ID must be positive.");
    }
    if (name_.empty()) {
        throw std::invalid_argument("Equipment name cannot be empty.");
    }
}

int getId() const { return id_; }
const std::string& getName() const { return name_; }
AccessCategory getCategory() const { return category_; }
bool isRestricted() const { return restricted_; }
bool isAvailable() const { return available_; }
void setAvailable(bool available) { available_ = available; }
};

struct EquipmentRecord {
int id;
std::string name;
std::string category;
bool isAvailable;
};

void printEquipment(const EquipmentRecord& equipment);
void printAllEquipment(const std::vector<EquipmentRecord>& equipment);
std::size_t countAvailableEquipment(const std::vector<EquipmentRecord>& equipment);
std::optional<std::size_t> findEquipmentIndexById(const std::vector<EquipmentRecord>& equipment, int id);
bool setEquipmentAvailability(std::vector<EquipmentRecord>& equipment, int id, bool isAvailable);
