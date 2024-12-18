#include <windows.h>
#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

vector<int> sharedArray;
vector<HANDLE> threadHandles;
vector<unique_ptr<atomic<bool>>> markerActive;
atomic<bool> startWork(false);
atomic<int> activeMarkers(0);
HANDLE canContinueEvent;
HANDLE allStoppedEvent;

struct MarkerParams {
    int threadIndex;
};

DWORD WINAPI MarkerThread(LPVOID lpParam) {
    auto* params = (MarkerParams*)lpParam;
    int threadIndex = params->threadIndex;
    int markedCount = 0;

    srand(threadIndex);

    while (true) {
        while (!startWork.load()) {
            this_thread::sleep_for(chrono::milliseconds(10));
        }

        int randValue = rand();
        int index = randValue % sharedArray.size();

        if (sharedArray[index] == 0) {
            this_thread::sleep_for(chrono::milliseconds(5));
            sharedArray[index] = threadIndex;
            markedCount++;
            this_thread::sleep_for(chrono::milliseconds(5));
        }
        else {
            cout << "Thread " << threadIndex << " cannot mark more elements.\n"
                << "Marked elements: " << markedCount << "\n"
                << "Conflict at index: " << index << "\n";

            activeMarkers--;
            if (activeMarkers.load() == 0) {
                SetEvent(allStoppedEvent);
            }

            WaitForSingleObject(canContinueEvent, INFINITE);
            ResetEvent(canContinueEvent);

            if (!markerActive[threadIndex]->load()) {
                for (auto& elem : sharedArray) {
                    if (elem == threadIndex) {
                        elem = 0;
                    }
                }
                return 0;
            }
        }
    }
}

int main() {
    int arraySize, markerCount;

    cout << "Enter the size of the array: ";
    cin >> arraySize;
    sharedArray.resize(arraySize, 0);

    cout << "Enter the number of marker threads: ";
    cin >> markerCount;

    markerActive.resize(markerCount);
    threadHandles.resize(markerCount);
    activeMarkers = markerCount;

    canContinueEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    allStoppedEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

    for (int i = 0; i < markerCount; ++i) {
        markerActive[i] = make_unique<atomic<bool>>(true);
        MarkerParams* params = new MarkerParams{ i };
        threadHandles[i] = CreateThread(nullptr, 0, MarkerThread, params, 0, nullptr);
    }

    startWork = true;

    while (activeMarkers.load() > 0) {
        WaitForSingleObject(allStoppedEvent, INFINITE);
        ResetEvent(allStoppedEvent);

        cout << "Array state: ";
        for (const auto& elem : sharedArray) {
            cout << elem << " ";
        }
        cout << "\n";

        int threadToTerminate;
        cout << "Enter the thread index to terminate: ";
        cin >> threadToTerminate;

        if (threadToTerminate >= 0 && threadToTerminate < markerCount) {
            markerActive[threadToTerminate]->store(false);
            activeMarkers--;
            SetEvent(canContinueEvent);

            WaitForSingleObject(threadHandles[threadToTerminate], INFINITE);
            CloseHandle(threadHandles[threadToTerminate]);

            cout << "Array state after termination: ";
            for (const auto& elem : sharedArray) {
                cout << elem << " ";
            }
            cout << "\n";
        }

        SetEvent(canContinueEvent);
    }

    for (auto handle : threadHandles) {
        CloseHandle(handle);
    }

    CloseHandle(canContinueEvent);
    CloseHandle(allStoppedEvent);

    cout << "All threads terminated. Exiting program.\n";
    return 0;
}
