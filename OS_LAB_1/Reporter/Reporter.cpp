#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <locale>

// Структура сотрудника
struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    if (argc != 4) {
        std::cerr << "Использование: Reporter <binary_file> <report_file> <hourly_rate>\n";
        return 1;
    }

    std::string binary_file = argv[1];
    std::string report_file = argv[2];
    double hourly_rate = std::atof(argv[3]);

    std::ifstream infile(binary_file, std::ios::binary);
    if (!infile) {
        std::cerr << "Ошибка при открытии бинарного файла.\n";
        return 1;
    }

    std::ofstream outfile(report_file);
    if (!outfile) {
        std::cerr << "Ошибка при создании файла отчета.\n";
        return 1;
    }

    outfile << "Отчет по файлу \"" << binary_file << "\":\n";
    outfile << "Номер сотрудника | Имя | Отработанные часы | Зарплата\n";
    outfile << "-----------------------------------------------------\n";

    std::cout << "Номер сотрудника | Имя | Отработанные часы | Зарплата\n";
    std::cout << "-----------------------------------------------------\n";

    employee emp;
    while (infile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        double salary = emp.hours * hourly_rate;

        outfile << std::setw(15) << emp.num << " | " << std::setw(10) << emp.name << " | "
            << std::setw(16) << emp.hours << " | " << std::setw(10) << salary << "\n";

        std::cout << std::setw(15) << emp.num << " | " << std::setw(10) << emp.name << " | "
            << std::setw(16) << emp.hours << " | " << std::setw(10) << salary << "\n";
    }

    infile.close();
    outfile.close();
    std::cout << "Отчет успешно сохранен в " << report_file << ".\n";

    return 0;
}