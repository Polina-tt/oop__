# Лабораторна робота №1

Налаштування середовища розробки, структура C++-проєкту та компіляція за допомогою GCC і CMake

## Інформація про студента
- **Прізвище, ім’я:** Щербак Поліна
- **Група:** 25.09
- **Обраний маршрут:** Windows + WSL

## Використані версії інструментів

**g++**  `15.2.0` (Ubuntu 15.2.0-16ubuntu1) 
**CMake**  `4.2.3` 
**GDB**  `17.1` (GNU gdb Ubuntu 17.1-2ubuntu1) 
---

## 1. Команди ручної компіляції

Послідовність команд для ручного препроцесингу, компіляції в об'єктні файли та компонування через `g++`:

```bash
# Створення каталогів для збірки
mkdir -p build/manual

# 1. Препроцесинг
g++ -std=c++20 -Iinclude -E src/equipment_record.cpp -o build/manual/equipment_record.ii

# 2. Генерування асемблерного коду
g++ -std=c++20 -Iinclude -S src/equipment_record.cpp -o build/manual/equipment_record.s

# 3. Створення об'єктних файлів
g++ -std=c++20 -Wall -Wextra -Wpedantic -Iinclude -c src/equipment_record.cpp -o build/manual/equipment_record.o
g++ -std=c++20 -Wall -Wextra -Wpedantic -Iinclude -c src/main.cpp -o build/manual/main.o

# 4. Компонування
g++ build/manual/main.o build/manual/equipment_record.o -o build/manual/campus_assets