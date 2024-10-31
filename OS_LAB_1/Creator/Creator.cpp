#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <locale>
#include <cassert> // Для assert

// Структура сотрудника
struct employee {
    int num;
    char name[10];
    double hours;
};

// Функция для юнит-тестирования
void runTest() {
    std::string test_file = "test_employee.bin";
    int test_records = 1;

    // Создаем файл с одной записью
    std::ofstream outfile(test_file, std::ios::binary);
    assert(outfile && "Ошибка при создании тестового файла.");

    employee test_emp = { 123, "Иван", 40.5 };
    outfile.write(reinterpret_cast<const char*>(&test_emp), sizeof(employee));
    outfile.close();

    // Проверяем запись
    std::ifstream infile(test_file, std::ios::binary);
    assert(infile && "Ошибка при открытии тестового файла.");

    employee read_emp;
    infile.read(reinterpret_cast<char*>(&read_emp), sizeof(employee));
    infile.close();

    // Проверка полей
    assert(read_emp.num == test_emp.num && "Неверный идентификационный номер.");
    assert(std::string(read_emp.name) == test_emp.name && "Неверное имя.");
    assert(read_emp.hours == test_emp.hours && "Неверное количество отработанных часов.");

    std::cout << "Тест пройден успешно.\n";
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc == 2 && std::string(argv[1]) == "test") {
        runTest();
        return 0;
    }

    if (argc != 3) {
        std::cerr << "Использование: Creator <binary_file> <num_records>\n";
        return 1;
    }

    std::string binary_file = argv[1];
    int num_records = std::atoi(argv[2]);

    std::ofstream outfile(binary_file, std::ios::binary);
    if (!outfile) {
        std::cerr << "Ошибка при создании бинарного файла.\n";
        return 1;
    }

    // Ввод данных сотрудников
    for (int i = 0; i < num_records; ++i) {
        employee emp;
        std::cout << "Введите данные сотрудника #" << (i + 1) << ":\n";
        std::cout << "Идентификационный номер: ";
        std::cin >> emp.num;
        std::cout << "Имя (до 10 символов): ";
        std::cin >> emp.name;
        std::cout << "Количество отработанных часов: ";
        std::cin >> emp.hours;

        // Запись сотрудника в бинарный файл
        outfile.write(reinterpret_cast<const char*>(&emp), sizeof(employee));
    }

    outfile.close();
    std::cout << "Данные сотрудников успешно сохранены в бинарный файл.\n";
    return 0;
}
