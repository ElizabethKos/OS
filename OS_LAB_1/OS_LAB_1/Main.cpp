#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>

// Функция для запуска процессов Creator и Reporter
void runProcess(const std::wstring& command) {
    setlocale(LC_ALL, "Russian");
    STARTUPINFO si = { sizeof(si) };  // Используем STARTUPINFO для Unicode
    PROCESS_INFORMATION pi;

    // Используем CreateProcessW для запуска процесса
    if (!CreateProcessW(NULL, const_cast<LPWSTR>(command.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        std::wcerr << L"Ошибка при запуске процесса: " << command << L"\n";
        exit(1);
    }

    // Ожидание завершения процесса
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Установка кодировок консоли
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    std::wstring binary_file, report_file;
    int num_records;
    double hourly_rate;

    // 1. Ввод имени бинарного файла и количества записей
    std::wcout << L"Введите имя бинарного файла: ";
    std::wcin >> binary_file;
    std::wcout << L"Введите количество записей: ";
    std::wcin >> num_records;

    // Запуск Creator для создания бинарного файла
    std::wstring creator_cmd = L"Creator " + binary_file + L" " + std::to_wstring(num_records);
    runProcess(creator_cmd);


    // 3. Ввод имени файла отчета и оплаты за час
    std::wcout << L"Введите имя файла отчета: ";
    std::wcin >> report_file;
    std::wcout << L"Введите оплату за час работы: ";
    std::wcin >> hourly_rate;

    // 4. Запуск Reporter для создания отчета
    std::wstring reporter_cmd = L"Reporter " + binary_file + L" " + report_file + L" " + std::to_wstring(hourly_rate);
    runProcess(reporter_cmd);

    return 0;
}