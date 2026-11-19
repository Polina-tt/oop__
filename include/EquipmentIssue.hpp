#pragma once

#include <stdexcept>
#include <memory>
#include "User.hpp"
#include "equipment_record.hpp" // Клас обладнання із правильним типом Equipment
#include "IEquipmentIssuancePolicy.hpp"

class EquipmentIssue {
private:
    int issue_id_;
    const User& user_;         // Асоціація (посилання на користувача)
    Equipment& equipment_;     // Асоціація (посилання на обладнання)
    bool is_active_;
    std::unique_ptr<IEquipmentIssuancePolicy> policy_; // Політика видачі

public:
    EquipmentIssue(int issue_id, const User& user, Equipment& equipment)
        : issue_id_(issue_id), user_(user), equipment_(equipment), is_active_(true) {
        if (issue_id_ <= 0) {
            throw std::invalid_argument("Issue ID must be positive.");
        }
    }

    int getId() const { return issue_id_; }
    const User& getUser() const { return user_; }
    Equipment& getEquipment() { return equipment_; }
    const Equipment& getEquipment() const { return equipment_; }
    bool isActive() const { return is_active_; }

    // Сеттер для встановлення політики під час виконання
    void setPolicy(std::unique_ptr<IEquipmentIssuancePolicy> newPolicy) {
        policy_ = std::move(newPolicy);
    }

    // Метод перевірки через інтерфейс (динамічний поліморфізм)
    IssuanceCheckResult validateIssuance() const {
        if (policy_) {
            return policy_->canIssue(user_, equipment_);
        }
        return {true, 0, "No policy set"};
    }
    
    // Закриття видачі (повернення обладнання)
    void close() {
        if (!is_active_) {
            throw std::logic_error("Issue is already closed.");
        }
        is_active_ = false;
    }
};