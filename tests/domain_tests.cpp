#include <catch2/catch_test_macros.hpp>
#include <stdexcept>
#include <vector>

#include "User.hpp"
#include "equipment_record.hpp"

TEST_CASE("Creates valid User and Equipment objects", "[domain]") {
    User user(1, "Polina", 3, UserRole::Staff, 0);
    Equipment eq(10, "Oscilloscope", AccessCategory::Laboratory, true, true);

    REQUIRE(user.getId() == 1);
    REQUIRE(user.getName() == "Polina");
    REQUIRE(eq.getId() == 10);
    REQUIRE(eq.isAvailable());
}

TEST_CASE("Rejects invalid constructor arguments with exceptions", "[domain][exceptions]") {
    REQUIRE_THROWS_AS(User(0, "Polina", 2), std::invalid_argument);
    REQUIRE_THROWS_AS(User(1, "", 2), std::invalid_argument);
    REQUIRE_THROWS_AS(Equipment(-5, "Scope"), std::invalid_argument);
    REQUIRE_THROWS_AS(Equipment(2, ""), std::invalid_argument);
}

TEST_CASE("Handles empty equipment list and missing search result", "[domain][boundary]") {
    std::vector<EquipmentRecord> empty_list;
    REQUIRE(countAvailableEquipment(empty_list) == 0);
    REQUIRE_FALSE(findEquipmentIndexById(empty_list, 1).has_value());
}