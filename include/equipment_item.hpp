#ifndef EQUIPMENT_ITEM_HPP
#define EQUIPMENT_ITEM_HPP

#include <string>

enum class EquipmentState {
    Available,
    Issued,
    UnderMaintenance
};

class EquipmentItem {
private:
    int id_;
    std::string name_;
    std::string category_;
    EquipmentState state_;

public:
    EquipmentItem(int id, std::string name, std::string category);

    int getId() const;
    const std::string& getName() const;
    const std::string& getCategory() const;
    EquipmentState getState() const;

    bool issue();
    bool returnItem();
    bool sendToMaintenance();
    bool completeMaintenance();
};

#endif // EQUIPMENT_ITEM_HPP