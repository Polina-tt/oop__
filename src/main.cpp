#include "equipment_item.hpp" // ОСЬ ЦЕЙ РЯДОК МАЄ БУТИ ПЕРШИМ
#include <cassert>
#include <iostream>
#include <stdexcept>

int main() {
   
    std::cout << "Starting behavioral checks...\n";

    // 1. Коректне створення об'єкта
    EquipmentItem item(1, "Oscilloscope", "Measuring");
    assert(item.getId() == 1);
    assert(item.getName() == "Oscilloscope");
    assert(item.getCategory() == "Measuring");
    assert(item.getState() == EquipmentState::Available);

    // 2. Спроба створення з некоректним ID (очікуємо виняток)
    bool invalidIdCaught = false;
    try {
        EquipmentItem badItem(-1, "Multimeter", "Electronics");
    } catch (const std::invalid_argument&) {
        invalidIdCaught = true;
    }
    assert(invalidIdCaught);

    // 3. Успішна видача та перевірка заборони повторної видачі
    assert(item.issue() == true);
    assert(item.getState() == EquipmentState::Issued);
    assert(item.issue() == false); // Повторна видача має бути заборонена

    // 4. Успішне повернення та перевірка заборони повторного повернення
    assert(item.returnItem() == true);
    assert(item.getState() == EquipmentState::Available);
    assert(item.returnItem() == false);

    // 5. Передача на обслуговування та заборона видачі під час ремонту
    assert(item.sendToMaintenance() == true);
    assert(item.getState() == EquipmentState::UnderMaintenance);
    assert(item.issue() == false); // Не можна видавати обладнання, що обслуговується

    // 6. Завершення обслуговування
    assert(item.completeMaintenance() == true);
    assert(item.getState() == EquipmentState::Available);

    std::cout << "All behavior checks passed successfully!\n";
    return 0;
}