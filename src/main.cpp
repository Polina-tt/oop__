#include <iostream>
#include <memory>
#include "User.hpp"
#include "equipment_record.hpp"
#include "EquipmentIssue.hpp"
#include "StandardIssuancePolicy.hpp"
#include "ShortTermIssuancePolicy.hpp"
#include "RestrictedEquipmentPolicy.hpp"

int main() {
User student(1, "Polina Student", 3, UserRole::Student, 0);
User staff(2, "Dr. Staff", 5, UserRole::Staff, 1);

Equipment laptop(101, "Laptop Dell", AccessCategory::Laboratory, false, true);
Equipment lab_scope(102, "Oscilloscope Pro", AccessCategory::HighSecurity, true, true);

EquipmentIssue issue(1, student, laptop);
issue.setPolicy(std::make_unique<StandardIssuancePolicy>());

auto r1 = issue.validateIssuance();
std::cout << "Standard (student, laptop): allowed=" << r1.allowed
          << ", days=" << r1.max_days << ", reason=" << r1.reason << "\n";

issue.setPolicy(std::make_unique<ShortTermIssuancePolicy>());
auto r2 = issue.validateIssuance();
std::cout << "ShortTerm (student, laptop): allowed=" << r2.allowed
          << ", days=" << r2.max_days << ", reason=" << r2.reason << "\n";

EquipmentIssue staff_issue(2, staff, lab_scope);
staff_issue.setPolicy(std::make_unique<RestrictedEquipmentPolicy>());
auto r3 = staff_issue.validateIssuance();
std::cout << "Restricted (staff, scope): allowed=" << r3.allowed
          << ", days=" << r3.max_days << ", reason=" << r3.reason << "\n";

return 0;
}
