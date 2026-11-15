#pragma once

#include <string>
#include <stdexcept>

class User {
private:
    int id_;
    std::string name_;
    size_t max_active_issues_;

public:
    explicit User(int id, std::string name, size_t max_active_issues)
        : id_(id), name_(std::move(name)), max_active_issues_(max_active_issues) {
        if (id_ <= 0) {
            throw std::invalid_argument("User ID must be positive.");
        }
        if (name_.empty()) {
            throw std::invalid_argument("User name cannot be empty.");
        }
        if (max_active_issues_ == 0) {
            throw std::invalid_argument("Max active issues must be greater than zero.");
        }
    }

    int getId() const { return id_; }
    const std::string& getName() const { return name_; }
    size_t getMaxActiveIssues() const { return max_active_issues_; }
};