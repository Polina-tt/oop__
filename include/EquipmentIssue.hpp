#pragma once

#include <stdexcept>
#include "User.hpp"
#include "EquipmentItem.hpp" // Клас із лабораторної №2

class EquipmentIssue {
private:
    int issue_id_;
    const User& user_;         // Асоціація (посилання на користувача)
    EquipmentItem& equipment_; // Асоціація (посилання на обладнання)
    bool is_active_;

public:
    EquipmentIssue(int issue_id, const User& user, EquipmentItem& equipment)
        : issue_id_(issue_id), user_(user), equipment_(equipment), is_active_(true) {
        if (issue_id_ <= 0) {
            throw std::invalid_argument("Issue ID must be positive.");
        }
    }

    int getId() const { return issue_id_; }
    const User& getUser() const { return user_; }
    EquipmentItem& getEquipment() { return equipment_; }
    const EquipmentItem& getEquipment() const { return equipment_; }
    bool isActive() const { return is_active_; }

    // Закриття видачі (повернення обладнання)
    void close() {
        if (!is_active_) {
            throw std::logic_error("Issue is already closed.");
        }
        is_active_ = false;
    }
};