#include <catch2/catch_test_macros.hpp>
#include <optional>

#include "IEquipmentIssuancePolicy.hpp"
#include "IssuanceCoordinator.hpp"
#include "RestrictedEquipmentPolicy.hpp"
#include "ShortTermIssuancePolicy.hpp"
#include "StandardIssuancePolicy.hpp"

namespace {

class AlwaysAllowPolicy : public IEquipmentIssuancePolicy {
public:
    mutable int calls = 0;

    IssuanceCheckResult canIssue(const User&, const Equipment&) const override {
        ++calls;
        return {true, 7, "Allowed by test policy"};
    }
};

class AlwaysDenyPolicy : public IEquipmentIssuancePolicy {
public:
    mutable int calls = 0;

    IssuanceCheckResult canIssue(const User&, const Equipment&) const override {
        ++calls;
        return {false, 0, "Denied by test policy"};
    }
};

} // namespace

TEST_CASE("Successful issue updates equipment availability", "[coordinator]") {
    StandardIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    std::optional<User> user = User(1, "Student", 3, UserRole::Student, 0);
    std::optional<Equipment> eq = Equipment(101, "Multimeter", AccessCategory::General, false, true);

    const IssueResult res = coordinator.issue(user, eq, 3);

    REQUIRE(res.ok);
    REQUIRE_FALSE(res.error.has_value());
    REQUIRE_FALSE(eq->isAvailable());
}

TEST_CASE("Fails when user is missing without changing equipment state", "[coordinator][state]") {
    StandardIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    std::optional<User> missing_user = std::nullopt;
    std::optional<Equipment> eq = Equipment(102, "Camera", AccessCategory::General, false, true);

    const IssueResult res = coordinator.issue(missing_user, eq, 2);

    REQUIRE_FALSE(res.ok);
    REQUIRE(res.error == IssueError::UserNotFound);
    REQUIRE(eq->isAvailable());
}

TEST_CASE("Fails when equipment is missing", "[coordinator][errors]") {
    StandardIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    std::optional<User> valid_user = User(1, "Student", 3);
    std::optional<Equipment> missing_eq = std::nullopt;

    const IssueResult res = coordinator.issue(valid_user, missing_eq, 2);

    REQUIRE_FALSE(res.ok);
    REQUIRE(res.error == IssueError::EquipmentNotFound);
}

TEST_CASE("Fails when equipment is already unavailable", "[coordinator][state]") {
    StandardIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    std::optional<User> user = User(2, "Student", 3);
    std::optional<Equipment> unavailable_eq = Equipment(103, "Analyzer", AccessCategory::General, false, false);

    const IssueResult res = coordinator.issue(user, unavailable_eq, 2);

    REQUIRE_FALSE(res.ok);
    REQUIRE(res.error == IssueError::EquipmentUnavailable);
    REQUIRE_FALSE(unavailable_eq->isAvailable());
}

TEST_CASE("Fails on non-positive loan period", "[coordinator][boundary]") {
    StandardIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    std::optional<User> user = User(2, "Student", 3);
    std::optional<Equipment> eq = Equipment(104, "Scope", AccessCategory::General, false, true);

    const IssueResult res = coordinator.issue(user, eq, 0);

    REQUIRE_FALSE(res.ok);
    REQUIRE(res.error == IssueError::InvalidPeriod);
    REQUIRE(eq->isAvailable());
}

TEST_CASE("Fails when requested duration exceeds policy limit", "[coordinator][boundary]") {
    StandardIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    std::optional<User> user = User(2, "Student", 3);
    std::optional<Equipment> eq = Equipment(104, "Scope", AccessCategory::General, false, true);

    const IssueResult res = coordinator.issue(user, eq, 30);

    REQUIRE_FALSE(res.ok);
    REQUIRE(res.error == IssueError::DurationExceedsLimit);
    REQUIRE(eq->isAvailable());
}

TEST_CASE("Fails when user loan limit is exceeded", "[coordinator][errors]") {
    StandardIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    std::optional<User> full_user = User(3, "Busy", 2, UserRole::Student, 2);
    std::optional<Equipment> eq = Equipment(105, "Tester", AccessCategory::General, false, true);

    const IssueResult res = coordinator.issue(full_user, eq, 2);

    REQUIRE_FALSE(res.ok);
    REQUIRE(res.error == IssueError::LoanLimitExceeded);
    REQUIRE(eq->isAvailable());
}

TEST_CASE("Uses AlwaysAllowPolicy test double to approve issuance", "[coordinator][test-double]") {
    AlwaysAllowPolicy allow_policy;
    IssuanceCoordinator allow_coord(allow_policy);
    std::optional<User> user = User(4, "Tester", 2);
    std::optional<Equipment> eq = Equipment(106, "Gen1", AccessCategory::General, false, true);

    const IssueResult res = allow_coord.issue(user, eq, 2);

    REQUIRE(res.ok);
    REQUIRE(allow_policy.calls == 1);
    REQUIRE_FALSE(eq->isAvailable());
}

TEST_CASE("Uses AlwaysDenyPolicy test double without altering equipment state", "[coordinator][test-double]") {
    AlwaysDenyPolicy deny_policy;
    IssuanceCoordinator deny_coord(deny_policy);
    std::optional<User> user = User(4, "Tester", 2);
    std::optional<Equipment> eq = Equipment(107, "Gen2", AccessCategory::General, false, true);

    const IssueResult res = deny_coord.issue(user, eq, 2);

    REQUIRE_FALSE(res.ok);
    REQUIRE(res.error == IssueError::PolicyRejected);
    REQUIRE(deny_policy.calls == 1);
    REQUIRE(eq->isAvailable());
}

TEST_CASE("ShortTermIssuancePolicy rejects restricted equipment", "[coordinator][policy]") {
    ShortTermIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    std::optional<User> user = User(5, "Student", 3, UserRole::Student, 0);
    std::optional<Equipment> restricted_eq = Equipment(108, "Laser", AccessCategory::Laboratory, true, true);

    const IssueResult res = coordinator.issue(user, restricted_eq, 2);

    REQUIRE_FALSE(res.ok);
    REQUIRE(res.error == IssueError::PolicyRejected);
    REQUIRE(restricted_eq->isAvailable());
}

TEST_CASE("RestrictedEquipmentPolicy rejects non-staff user", "[coordinator][policy]") {
    RestrictedEquipmentPolicy policy;
    IssuanceCoordinator coordinator(policy);
    std::optional<User> student = User(6, "Student", 3, UserRole::Student, 0);
    std::optional<Equipment> restricted_eq = Equipment(109, "Analyzer", AccessCategory::Laboratory, true, true);

    const IssueResult res = coordinator.issue(student, restricted_eq, 2);

    REQUIRE_FALSE(res.ok);
    REQUIRE(res.error == IssueError::PolicyRejected);
    REQUIRE(student->getActiveIssues() == 0);
    REQUIRE(restricted_eq->isAvailable());
}

TEST_CASE("Successful return restores equipment availability", "[coordinator][return]") {
    StandardIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    Equipment eq(110, "DMM", AccessCategory::General, false, false);

    const IssueResult res = coordinator.returnItem(eq, true, false, false);

    REQUIRE(res.ok);
    REQUIRE(eq.isAvailable());
}

TEST_CASE("Repeated return is rejected with LoanAlreadyClosed", "[coordinator][return]") {
    StandardIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    Equipment eq(111, "DMM2", AccessCategory::General, false, true);

    const IssueResult res = coordinator.returnItem(eq, false, true, false);

    REQUIRE_FALSE(res.ok);
    REQUIRE(res.error == IssueError::LoanAlreadyClosed);
}

TEST_CASE("Missing loan on return is rejected with LoanNotFound", "[coordinator][return]") {
    StandardIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    Equipment eq(112, "DMM3", AccessCategory::General, false, true);

    const IssueResult res = coordinator.returnItem(eq, false, false, false);

    REQUIRE_FALSE(res.ok);
    REQUIRE(res.error == IssueError::LoanNotFound);
}

TEST_CASE("Returning mismatched equipment is rejected with WrongEquipmentReturned", "[coordinator][return]") {
    StandardIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    Equipment eq(113, "DMM4", AccessCategory::General, false, false);

    const IssueResult res = coordinator.returnItem(eq, true, false, true);

    REQUIRE_FALSE(res.ok);
    REQUIRE(res.error == IssueError::WrongEquipmentReturned);
    REQUIRE_FALSE(eq.isAvailable());
}

TEST_CASE("Sequential issue and return cycle works properly", "[coordinator][sequence]") {
    StandardIssuancePolicy policy;
    IssuanceCoordinator coordinator(policy);
    std::optional<User> user = User(7, "Student", 2, UserRole::Student, 0);
    std::optional<Equipment> eq = Equipment(114, "Generator", AccessCategory::General, false, true);

    const IssueResult first_issue = coordinator.issue(user, eq, 4);
    REQUIRE(first_issue.ok);
    REQUIRE_FALSE(eq->isAvailable());

    const IssueResult returned = coordinator.returnItem(*eq, true, false, false);
    REQUIRE(returned.ok);
    REQUIRE(eq->isAvailable());

    const IssueResult second_issue = coordinator.issue(user, eq, 2);
    REQUIRE(second_issue.ok);
    REQUIRE_FALSE(eq->isAvailable());
}