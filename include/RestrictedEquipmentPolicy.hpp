#pragma once

#include "IEquipmentIssuancePolicy.hpp"
#include "User.hpp"
#include "equipment_record.hpp"

class RestrictedEquipmentPolicy : public IEquipmentIssuancePolicy {
private:
static constexpr int kRestrictedDays = 5;
static constexpr size_t kRestrictedMaxActive = 2;

public:
IssuanceCheckResult canIssue(const User& user, const Equipment& equipment) const override {
    if (!equipment.isAvailable()) {
        return {false, 0, "Restricted equipment is currently unavailable."};
    }
    if (user.getRole() != UserRole::Staff) {
        return {false, 0, "Access denied: only staff can issue restricted equipment."};
    }
    if (equipment.getCategory() == AccessCategory::General && !equipment.isRestricted()) {
        return {false, 0, "Equipment is not marked as restricted."};
    }
    if (user.getActiveIssues() >= kRestrictedMaxActive) {
        return {false, 0, "Restricted policy limit of active issues exceeded."};
    }
    return {true, kRestrictedDays, "Approved for staff under restricted equipment policy (up to 5 days)."};
}
};
