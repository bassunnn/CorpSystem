#include "TaskManager.h"
#include "TimeTracker.h"

#include <iostream>

int main()
{
    TaskManager taskManager;
    const int taskId = taskManager.addTask("Prepare lab", "Write code and report");

    TimeTracker tracker(taskManager);
    if (!tracker.start(taskId)) {
        std::cerr << "Failed to start timer\n";
        return 1;
    }

    const auto stopped = tracker.stop();
    if (!stopped.has_value()) {
        std::cerr << "Failed to stop timer\n";
        return 1;
    }

    std::cout << "Basic workflow passed: task #" << taskId
              << ", session #" << stopped->id << "\n";
    return 0;
}
