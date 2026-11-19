#pragma once

#include "IEquipmentIssuancePolicy.hpp"
#include "User.hpp"
#include "equipment_record.hpp"

class ShortTermIssuancePolicy : public IEquipmentIssuancePolicy {
private:
static constexpr int kShortTermDays = 3;
static constexpr size_t kShortTermMaxActive = 1;

public:
IssuanceCheckResult canIssue(const User& user, const Equipment& equipment) const override {
    if (!equipment.isAvailable()) {
        return {false, 0, "Equipment is currently unavailable."};
    }
    if (equipment.isRestricted()) {
        return {false, 0, "Restricted equipment cannot be issued under short-term policy."};
    }
    if (user.getActiveIssues() >= kShortTermMaxActive) {
        return {false, 0, "Short-term policy allows at most 1 active issue."};
    }
    return {true, kShortTermDays, "Approved by short-term policy (up to 3 days)."};
}
};
