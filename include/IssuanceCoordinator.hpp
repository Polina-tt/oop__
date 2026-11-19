#pragma once

#include <optional>
#include <string>
#include <vector>

#include "IEquipmentIssuancePolicy.hpp"
#include "IssuanceResult.hpp"
#include "IssueRegistry.hpp"
#include "User.hpp"
#include "equipment_record.hpp"

class IssuanceCoordinator {
private:
    const IEquipmentIssuancePolicy& policy_;
    IssueRegistry registry_;

public:
    explicit IssuanceCoordinator(const IEquipmentIssuancePolicy& policy)
        : policy_(policy) {}

    IssueResult issue(
        const std::optional<User>& user_opt,
        std::optional<Equipment>& equipment_opt,
        int requested_days,
        int expected_equipment_id = -1
    ) {
        if (!user_opt.has_value()) {
            return IssueResult::failure(IssueError::UserNotFound, "User not found.");
        }
        if (!equipment_opt.has_value()) {
            return IssueResult::failure(IssueError::EquipmentNotFound, "Equipment not found.");
        }

        const User& user = *user_opt;
        Equipment& equipment = *equipment_opt;

        if (expected_equipment_id > 0 && equipment.getId() != expected_equipment_id) {
            return IssueResult::failure(IssueError::EquipmentNotFound, "Equipment ID mismatch.");
        }
        if (requested_days <= 0) {
            return IssueResult::failure(IssueError::InvalidPeriod, "Requested period must be positive.");
        }
        if (!equipment.isAvailable()) {
            return IssueResult::failure(IssueError::EquipmentUnavailable, "Equipment is unavailable.");
        }
        if (user.getActiveIssues() >= user.getMaxActiveIssues() ||
            registry_.getActiveCountForUser(user.getId()) >= user.getMaxActiveIssues()) {
            return IssueResult::failure(IssueError::LoanLimitExceeded, "User loan limit exceeded.");
        }

        const IssuanceCheckResult check = policy_.canIssue(user, equipment);
        if (!check.allowed) {
            return IssueResult::failure(IssueError::PolicyRejected, check.reason);
        }
        if (check.max_days > 0 && requested_days > check.max_days) {
            return IssueResult::failure(
                IssueError::DurationExceedsLimit,
                "Requested duration exceeds policy maximum."
            );
        }

        // Усі перевірки пройдені — змінюємо стан лише тепер
        equipment.setAvailable(false);
        return IssueResult::success(1, "Equipment issued successfully.");
    }

    IssueResult returnItem(Equipment& equipment, bool has_active_loan, bool already_closed, bool wrong_equipment = false) {
        if (wrong_equipment) {
            return IssueResult::failure(
                IssueError::WrongEquipmentReturned,
                "Returned equipment does not match issued equipment."
            );
        }
        if (!has_active_loan && !already_closed) {
            return IssueResult::failure(IssueError::LoanNotFound, "Loan record not found.");
        }
        if (already_closed) {
            return IssueResult::failure(IssueError::LoanAlreadyClosed, "Loan is already closed.");
        }

        equipment.setAvailable(true);
        return IssueResult::success(1, "Equipment returned successfully.");
    }

    bool hasActiveLoanForEquipment(int equipment_id) const {
        for (const auto& issue : registry_.getIssues()) {
            if (issue.isActive() && issue.getEquipment().getId() == equipment_id) {
                return true;
            }
        }
        return false;
    }

    std::size_t issuedCount() const {
        return registry_.getIssues().size();
    }
};