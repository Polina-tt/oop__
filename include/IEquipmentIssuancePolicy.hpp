#pragma once

#include <string>

class User;
class Equipment;

struct IssuanceCheckResult {
bool allowed;
int max_days;
std::string reason;
};

class IEquipmentIssuancePolicy {
public:
virtual ~IEquipmentIssuancePolicy() = default;

virtual IssuanceCheckResult canIssue(const User& user, const Equipment& equipment) const = 0;
};
