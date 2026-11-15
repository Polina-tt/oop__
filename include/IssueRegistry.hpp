#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>
#include "EquipmentIssue.hpp"

class IssueRegistry {
private:
    std::vector<EquipmentIssue> issues_; // Композиція: реєстр володіє записами
    int next_issue_id_ = 1;

public:
    IssueRegistry() = default;

    // Підрахунок активних видач для конкретного користувача
    size_t getActiveCountForUser(int user_id) const {
        return std::count_if(issues_.begin(), issues_.end(), [user_id](const EquipmentIssue& issue) {
            return issue.isActive() && issue.getUser().getId() == user_id;
        });
    }

    // Видача обладнання з перевіркою всіх правил
    void issueEquipment(const User& user, EquipmentItem& equipment) {
        // 1. Перевірка: чи обладнання вже видане
        auto it = std::find_if(issues_.begin(), issues_.end(), [&](const EquipmentIssue& issue) {
            return issue.isActive() && issue.getEquipment().getId() == equipment.getId();
        });
        if (it != issues_.end()) {
            throw std::runtime_error("Equipment is already issued.");
        }

        // 2. Перевірка: чи не перевищено ліміт користувача
        if (getActiveCountForUser(user.getId()) >= user.getMaxActiveIssues()) {
            throw std::runtime_error("User has reached the maximum active issues limit.");
        }

        // 3. Додавання нового запису
        issues_.emplace_back(next_issue_id_++, user, equipment);
    }

    // Повернення обладнання
    void returnEquipment(int equipment_id) {
        auto it = std::find_if(issues_.begin(), issues_.end(), [&](EquipmentIssue& issue) {
            return issue.isActive() && issue.getEquipment().getId() == equipment_id;
        });
        if (it == issues_.end()) {
            throw std::runtime_error("No active issue found for this equipment.");
        }
        it->close();
    }

    // Отримання всіх записів (для перевірки та тестів)
    const std::vector<EquipmentIssue>& getIssues() const {
        return issues_;
    }
};