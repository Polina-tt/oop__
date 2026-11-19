#pragma once

#include "IEquipmentIssuancePolicy.hpp"
#include "User.hpp"
#include "equipment_record.hpp"

class StandardIssuancePolicy : public IEquipmentIssuancePolicy {
private:
static constexpr int kStandardDays = 14;

public:
IssuanceCheckResult canIssue(const User& user, const Equipment& equipment) const override {
    if (!equipment.isAvailable()) {
        return {false, 0, "Equipment is currently unavailable."};
    }
    if (user.getActiveIssues() >= user.getMaxActiveIssues()) {
        return {false, 0, "Active issue limit reached for user."};
    }
    if (equipment.isRestricted() && user.getRole() != UserRole::Staff) {
        return {false, 0, "Restricted equipment requires staff role under standard policy."};
    }
    return {true, kStandardDays, "Approved by standard policy (up to 14 days)."};
}
};
