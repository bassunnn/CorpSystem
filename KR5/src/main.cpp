#include "ConsoleUI.h"

#include <iostream>
#include <string>
#include <vector>

namespace {
void printHelp()
{
    std::cout << "Time Tracker\n"
              << "Usage:\n"
              << "  time_tracker                         interactive menu\n"
              << "  time_tracker --demo                  run container demo\n"
              << "  time_tracker [--data DIR] add-task TITLE [DESCRIPTION]\n"
              << "  time_tracker [--data DIR] list-tasks\n"
              << "  time_tracker [--data DIR] archive-task ID\n"
              << "  time_tracker [--data DIR] delete-task ID\n"
              << "  time_tracker [--data DIR] start ID\n"
              << "  time_tracker [--data DIR] stop\n"
              << "  time_tracker [--data DIR] history\n"
              << "  time_tracker [--data DIR] report\n";
}

void printTasks(const TaskManager& taskManager)
{
    std::cout << "\nTasks\n";
    std::cout << "-----\n";
    for (const auto& task : taskManager.tasks()) {
        std::cout << '#' << task.id << " [" << toString(task.status) << "] "
                  << task.title << " - " << task.description << '\n';
    }
}

void printHistory(const TimeTracker& timeTracker)
{
    std::cout << "\nSessions\n";
    std::cout << "--------\n";
    for (const auto& session : timeTracker.sessions()) {
        std::cout << '#' << session.id
                  << " task #" << session.taskId
                  << " duration " << session.durationSeconds() << " seconds"
                  << (session.active ? " (active)" : "") << '\n';
    }
}

void save(Storage& storage, const TaskManager& taskManager, const TimeTracker& timeTracker)
{
    storage.saveTasks(taskManager.tasks());
    storage.saveSessions(timeTracker.sessions());
}

int runCommand(
    const std::vector<std::string>& args,
    TaskManager& taskManager,
    TimeTracker& timeTracker,
    Storage& storage)
{
    if (args.empty()) {
        ConsoleUI ui(taskManager, timeTracker, storage);
        ui.run();
        return 0;
    }

    const std::string& command = args.front();
    if (command == "--help" || command == "help") {
        printHelp();
        return 0;
    }

    if (command == "--demo") {
        if (taskManager.tasks().empty()) {
            taskManager.addTask("Container demo", "Task created inside Docker container");
        }
        printTasks(taskManager);
        ReportService reportService;
        reportService.printReport(taskManager.tasks(), timeTracker.sessions());
        save(storage, taskManager, timeTracker);
        return 0;
    }

    if (command == "add-task") {
        if (args.size() < 2) {
            std::cerr << "add-task requires TITLE argument.\n";
            return 1;
        }
        const std::string description = args.size() >= 3 ? args[2] : "";
        const int id = taskManager.addTask(args[1], description);
        save(storage, taskManager, timeTracker);
        std::cout << "Task #" << id << " added.\n";
        return 0;
    }

    if (command == "list-tasks") {
        printTasks(taskManager);
        return 0;
    }

    if (command == "archive-task") {
        if (args.size() < 2) {
            std::cerr << "archive-task requires ID argument.\n";
            return 1;
        }
        if (!taskManager.archiveTask(std::stoi(args[1]))) {
            std::cerr << "Task not found.\n";
            return 1;
        }
        save(storage, taskManager, timeTracker);
        std::cout << "Task archived.\n";
        return 0;
    }

    if (command == "delete-task") {
        if (args.size() < 2) {
            std::cerr << "delete-task requires ID argument.\n";
            return 1;
        }
        const int taskId = std::stoi(args[1]);
        if (!taskManager.deleteTask(taskId)) {
            std::cerr << "Task not found.\n";
            return 1;
        }
        const int removedSessions = timeTracker.deleteSessionsForTask(taskId);
        save(storage, taskManager, timeTracker);
        std::cout << "Task deleted. Removed sessions: " << removedSessions << ".\n";
        return 0;
    }

    if (command == "start") {
        if (args.size() < 2) {
            std::cerr << "start requires ID argument.\n";
            return 1;
        }
        if (!timeTracker.start(std::stoi(args[1]))) {
            std::cerr << "Could not start timer.\n";
            return 1;
        }
        save(storage, taskManager, timeTracker);
        std::cout << "Timer started.\n";
        return 0;
    }

    if (command == "stop") {
        const auto session = timeTracker.stop();
        if (!session.has_value()) {
            std::cerr << "No active timer.\n";
            return 1;
        }
        save(storage, taskManager, timeTracker);
        std::cout << "Timer stopped. Duration: " << session->durationSeconds() << " seconds.\n";
        return 0;
    }

    if (command == "history") {
        printHistory(timeTracker);
        return 0;
    }

    if (command == "report") {
        ReportService reportService;
        reportService.printReport(taskManager.tasks(), timeTracker.sessions());
        return 0;
    }

    std::cerr << "Unknown command: " << command << '\n';
    printHelp();
    return 1;
}
}

int main(int argc, char* argv[])
{
    std::string dataDirectory = "data";
    std::vector<std::string> args;
    for (int index = 1; index < argc; ++index) {
        std::string value = argv[index];
        if (value == "--data") {
            if (index + 1 >= argc) {
                std::cerr << "--data requires directory argument.\n";
                return 1;
            }
            dataDirectory = argv[++index];
        } else {
            args.push_back(value);
        }
    }

    TaskManager taskManager;
    Storage storage(dataDirectory);
    taskManager.replaceTasks(storage.loadTasks());

    TimeTracker timeTracker(taskManager);
    timeTracker.replaceSessions(storage.loadSessions());

    return runCommand(args, taskManager, timeTracker, storage);
}
