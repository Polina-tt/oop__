#pragma once

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

#include "EquipmentIssue.hpp"
#include "IssuanceResult.hpp"
#include "User.hpp"
#include "equipment_record.hpp"

class IssueRegistry {
private:
    std::vector<EquipmentIssue> issues_;
    int next_issue_id_ = 1;

public:
    IssueRegistry() = default;

    std::size_t getActiveCountForUser(int user_id) const {
        return static_cast<std::size_t>(std::count_if(
            issues_.begin(),
            issues_.end(),
            [user_id](const EquipmentIssue& issue) {
                return issue.isActive() && issue.getUser().getId() == user_id;
            }
        ));
    }

    const EquipmentIssue* findIssueById(int issue_id) const {
        auto it = std::find_if(
            issues_.begin(),
            issues_.end(),
            [issue_id](const EquipmentIssue& issue) {
                return issue.getId() == issue_id;
            }
        );
        if (it == issues_.end()) {
            return nullptr;
        }
        return &(*it);
    }

    IssueResult issueEquipment(
        const User& user,
        Equipment& equipment,
        int requested_days = 1,
        int max_allowed_days = 14
    ) {
        if (requested_days <= 0) {
            return IssueResult::failure(
                IssueError::InvalidPeriod,
                "Requested loan period must be positive."
            );
        }
        if (requested_days > max_allowed_days) {
            return IssueResult::failure(
                IssueError::DurationExceedsLimit,
                "Requested loan duration exceeds allowed limit."
            );
        }
        if (!equipment.isAvailable()) {
            return IssueResult::failure(
                IssueError::EquipmentUnavailable,
                "Equipment is currently unavailable."
            );
        }

        auto active_it = std::find_if(
            issues_.begin(),
            issues_.end(),
            [&equipment](const EquipmentIssue& issue) {
                return issue.isActive() && issue.getEquipment().getId() == equipment.getId();
            }
        );
        if (active_it != issues_.end()) {
            return IssueResult::failure(
                IssueError::EquipmentUnavailable,
                "Equipment already has an active issue."
            );
        }

        if (getActiveCountForUser(user.getId()) >= user.getMaxActiveIssues()) {
            return IssueResult::failure(
                IssueError::LoanLimitExceeded,
                "User has reached the maximum active issues limit."
            );
        }

        const int assigned_id = next_issue_id_++;
        equipment.setAvailable(false);
        issues_.emplace_back(assigned_id, user, equipment);
        return IssueResult::success(assigned_id, "Equipment issued successfully.");
    }

    IssueResult returnEquipment(int equipment_id) {
        auto any_it = std::find_if(
            issues_.begin(),
            issues_.end(),
            [equipment_id](const EquipmentIssue& issue) {
                return issue.getEquipment().getId() == equipment_id;
            }
        );
        if (any_it == issues_.end()) {
            return IssueResult::failure(
                IssueError::LoanNotFound,
                "No issue record found for this equipment."
            );
        }
        if (!any_it->isActive()) {
            return IssueResult::failure(
                IssueError::LoanAlreadyClosed,
                "Issue for this equipment is already closed."
            );
        }

        any_it->close();
        return IssueResult::success(any_it->getId(), "Equipment returned successfully.");
    }

    const std::vector<EquipmentIssue>& getIssues() const {
        return issues_;
    }
};