#define CATCH_CONFIG_MAIN
#include <catch_all.hpp>
#include <vector>
#include <numeric>
#include <thread>
#include <future>

// Глобальные переменные для хранения результатов
int minValue, maxValue;
double averageValue;

// Определение функций, которые будут тестироваться
void findMinMax(const std::vector<int>& arr) {
    minValue = arr[0];
    maxValue = arr[0];

    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < minValue) minValue = arr[i];
        if (arr[i] > maxValue) maxValue = arr[i];
    }
}

double calculateAverage(const std::vector<int>& arr) {
    int sum = std::accumulate(arr.begin(), arr.end(), 0);
    return static_cast<double>(sum) / arr.size();
}

TEST_CASE("Min and max values") {
    std::vector<int> arr = { 3, 5, 1, 9, 2 };

    findMinMax(arr);

    REQUIRE(minValue == 1);
    REQUIRE(maxValue == 9);
}

TEST_CASE("Average value") {
    std::vector<int> arr = { 3, 5, 1, 9, 2 };

    averageValue = calculateAverage(arr);

    REQUIRE(averageValue == Approx(4.0)); // Используем Approx для сравнения с плавающей точкой
}

TEST_CASE("Modified array") {
    std::vector<int> arr = { 3, 5, 1, 9, 2 };

    findMinMax(arr);
    averageValue = calculateAverage(arr);

    for (int& num : arr) {
        if (num == minValue || num == maxValue) {
            num = static_cast<int>(averageValue);
        }
    }

    REQUIRE(arr[0] == 4);
    REQUIRE(arr[1] == 5);
    REQUIRE(arr[2] == 4);
    REQUIRE(arr[3] == 4);
    REQUIRE(arr[4] == 4);
}