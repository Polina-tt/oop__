#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <optional>
#include "TimeSlot.h" // Підключаємо наш новий тип-значення

int main() {
    try {
        std::cout << "=== LABORATORY WORK 4: Value Semantics & STL ==_\n\n";

        // 1. Створюємо колекцію (std::vector) за допомогою нашого типу-значення TimeSlot
        std::vector<TimeSlot> schedule;
        schedule.emplace_back(9, 11);
        schedule.emplace_back(12, 14);
        schedule.emplace_back(15, 17);
        schedule.emplace_back(10, 12); // Цей проміжок буде перетинатися з першим

        std::cout << "Initial schedule slots count: " << schedule.size() << "\n";

        // 2. Використання стандартного алгоритму STL: std::find_if для пошуку проміжку
        // Шукаємо слот, у якого тривалість дорівнює 3 годинам
        auto it = std::find_if(schedule.begin(), schedule.end(), [](const TimeSlot& slot) {
            return slot.getDuration() == 3;
        });

        if (it != schedule.end()) {
            std::cout << "Found slot with duration 3h: start at " << it->getStartHour() << ":00\n";
        } else {
            std::cout << "Slot with duration 3h not found.\n";
        }

        // 3. Використання стандартного алгоритму STL: сортування (std::sort)
        // Завдяки оператору <=> у TimeSlot, сортування працює автоматично за часом початку
        std::sort(schedule.begin(), schedule.end());
        std::cout << "\nSchedule sorted chronologically.\n";

        // 4. Демонстрація роботи з опціональним результатом (std::optional) замість -1 чи вказівників
        auto findShortestSlot = [&schedule]() -> std::optional<TimeSlot> {
            if (schedule.empty()) return std::nullopt;
            
            auto min_it = std::min_element(schedule.begin(), schedule.end(), [](const TimeSlot& a, const TimeSlot& b) {
                return a.getDuration() < b.getDuration();
            });
            return *min_it;
        };

        std::optional<TimeSlot> shortest = findShortestSlot();
        if (shortest) {
            std::cout << "Shortest slot duration: " << shortest->getDuration() << " hour(s)\n";
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}