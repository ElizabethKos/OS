#include <gtest/gtest.h>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

vector<int> testArray;
atomic<bool> testStartWork(false);
atomic<int> testActiveMarkers(0);

void MarkerThreadTest(int threadIndex, atomic<bool>& activeFlag, int arraySize) {
    srand(threadIndex);
    int markedCount = 0;

    while (true) {
        while (!testStartWork.load()) {
            this_thread::sleep_for(chrono::milliseconds(10));
        }

        int randValue = rand();
        int index = randValue % arraySize;

        if (testArray[index] == 0) {
            this_thread::sleep_for(chrono::milliseconds(5));
            testArray[index] = threadIndex;
            markedCount++;
            this_thread::sleep_for(chrono::milliseconds(5));
        }
        else {
            testActiveMarkers--;
            if (!activeFlag.load()) {
                for (auto& elem : testArray) {
                    if (elem == threadIndex) {
                        elem = 0;
                    }
                }
                return;
            }
        }
    }
}

class MarkerThreadTests : public ::testing::Test {
protected:
    void SetUp() override {
        testArray.clear();
        testStartWork = false;
        testActiveMarkers = 0;
    }
};

TEST_F(MarkerThreadTests, TestArrayInitialization) {
    int arraySize = 10;
    testArray.resize(arraySize, 0);

    for (const auto& elem : testArray) {
        ASSERT_EQ(elem, 0);
    }
}

TEST_F(MarkerThreadTests, TestMultipleThreads) {
    int arraySize = 20;
    testArray.resize(arraySize, 0);
    testActiveMarkers = 3;
    testStartWork = true;

    vector<thread> threads;
    vector<atomic<bool>> activeFlags(3);

    for (int i = 0; i < 3; ++i) {
        activeFlags[i] = true;
        threads.emplace_back(MarkerThreadTest, i + 1, ref(activeFlags[i]), arraySize);
    }

    this_thread::sleep_for(chrono::milliseconds(100));
    activeFlags[0] = false;

    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    int totalMarked = 0;
    for (const auto& elem : testArray) {
        if (elem > 0) {
            totalMarked++;
        }
    }

    ASSERT_GT(totalMarked, 0);
    ASSERT_EQ(testActiveMarkers.load(), 0);
}
//int main(int argc, char** argv) {
//    ::testing::InitGoogleTest(&argc, argv);  // Инициализация Google Test
//    return RUN_ALL_TESTS();  // Запуск всех тестов
//}