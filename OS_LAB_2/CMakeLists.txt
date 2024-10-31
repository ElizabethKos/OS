cmake_minimum_required(VERSION 3.14)
project(OS_LAB_2)

# Подключение Catch2
add_subdirectory(external/Catch2)

# Создание исполняемого файла
add_executable(OS_LAB_2 main.cpp)
target_link_libraries(OS_LAB_2 Catch2::Catch2)
