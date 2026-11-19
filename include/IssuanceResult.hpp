#pragma once

#include <optional>
#include <string>

enum class IssueError {
    UserNotFound,
    EquipmentNotFound,
    EquipmentUnavailable,
    LoanLimitExceeded,
    PolicyRejected,
    DurationExceedsLimit,
    LoanNotFound,
    LoanAlreadyClosed,
    InvalidPeriod,
    WrongEquipmentReturned
};

struct IssueResult {
    bool ok = false;
    std::optional<IssueError> error;
    std::string message;
    int issue_id = 0;

    static IssueResult success(int created_issue_id = 0, std::string msg = "Operation succeeded.") {
        return IssueResult{true, std::nullopt, std::move(msg), created_issue_id};
    }

    static IssueResult failure(IssueError code, std::string msg) {
        return IssueResult{false, code, std::move(msg), 0};
    }
};