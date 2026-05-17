#include "ConsoleUI.h"

#include <iostream>
#include <limits>

namespace {
int readInt()
{
    int value{};
    std::cin >> value;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return value;
}
}

ConsoleUI::ConsoleUI(TaskManager& taskManager, TimeTracker& timeTracker, Storage& storage)
    : taskManager_(taskManager),
      timeTracker_(timeTracker),
      storage_(storage)
{
}

void ConsoleUI::run()
{
    bool running = true;
    while (running) {
        std::cout << "\nTime Tracker\n";
        std::cout << "1. Add task\n";
        std::cout << "2. List tasks\n";
        std::cout << "3. Archive task\n";
        std::cout << "4. Start timer\n";
        std::cout << "5. Stop timer\n";
        std::cout << "6. Show history\n";
        std::cout << "7. Show report\n";
        std::cout << "0. Save and exit\n";
        std::cout << "Choice: ";

        switch (readInt()) {
        case 1:
            addTask();
            break;
        case 2:
            listTasks();
            break;
        case 3:
            archiveTask();
            break;
        case 4:
            startTimer();
            break;
        case 5:
            stopTimer();
            break;
        case 6:
            showHistory();
            break;
        case 7:
            reportService_.printReport(taskManager_.tasks(), timeTracker_.sessions());
            break;
        case 0:
            save();
            running = false;
            break;
        default:
            std::cout << "Unknown command.\n";
            break;
        }
    }
}

void ConsoleUI::addTask()
{
    std::string title;
    std::string description;

    std::cout << "Title: ";
    std::getline(std::cin, title);
    std::cout << "Description: ";
    std::getline(std::cin, description);

    const int id = taskManager_.addTask(title, description);
    std::cout << "Task #" << id << " added.\n";
}

void ConsoleUI::listTasks() const
{
    std::cout << "\nTasks\n";
    std::cout << "-----\n";
    for (const auto& task : taskManager_.tasks()) {
        std::cout << '#' << task.id << " [" << toString(task.status) << "] "
                  << task.title << " - " << task.description << '\n';
    }
}

void ConsoleUI::archiveTask()
{
    std::cout << "Task id: ";
    if (taskManager_.archiveTask(readInt())) {
        std::cout << "Task archived.\n";
    } else {
        std::cout << "Task not found.\n";
    }
}

void ConsoleUI::startTimer()
{
    std::cout << "Task id: ";
    if (timeTracker_.start(readInt())) {
        std::cout << "Timer started.\n";
    } else {
        std::cout << "Could not start timer.\n";
    }
}

void ConsoleUI::stopTimer()
{
    const auto session = timeTracker_.stop();
    if (session.has_value()) {
        std::cout << "Timer stopped. Duration: " << session->durationSeconds() << " seconds.\n";
    } else {
        std::cout << "No active timer.\n";
    }
}

void ConsoleUI::showHistory() const
{
    std::cout << "\nSessions\n";
    std::cout << "--------\n";
    for (const auto& session : timeTracker_.sessions()) {
        std::cout << '#' << session.id
                  << " task #" << session.taskId
                  << " duration " << session.durationSeconds() << " seconds"
                  << (session.active ? " (active)" : "") << '\n';
    }
}

void ConsoleUI::save() const
{
    storage_.saveTasks(taskManager_.tasks());
    storage_.saveSessions(timeTracker_.sessions());
    std::cout << "Data saved.\n";
}
