#include "equipment_item.hpp"
#include <stdexcept>

EquipmentItem::EquipmentItem(int id, std::string name, std::string category)
    : id_(id), name_(std::move(name)), category_(std::move(category)), state_(EquipmentState::Available) {
    if (id_ <= 0) {
        throw std::invalid_argument("Error: ID must be greater than zero.");
    }
    if (name_.empty() || category_.empty()) {
        throw std::invalid_argument("Error: Name and category cannot be empty.");
    }
}

int EquipmentItem::getId() const {
    return id_;
}

const std::string& EquipmentItem::getName() const {
    return name_;
}

const std::string& EquipmentItem::getCategory() const {
    return category_;
}

EquipmentState EquipmentItem::getState() const {
    return state_;
}

bool EquipmentItem::issue() {
    if (state_ == EquipmentState::Available) {
        state_ = EquipmentState::Issued;
        return true;
    }
    return false;
}

bool EquipmentItem::returnItem() {
    if (state_ == EquipmentState::Issued) {
        state_ = EquipmentState::Available;
        return true;
    }
    return false;
}

bool EquipmentItem::sendToMaintenance() {
    if (state_ == EquipmentState::Available) {
        state_ = EquipmentState::UnderMaintenance;
        return true;
    }
    return false;
}

bool EquipmentItem::completeMaintenance() {
    if (state_ == EquipmentState::UnderMaintenance) {
        state_ = EquipmentState::Available;
        return true;
    }
    return false;
}