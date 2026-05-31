#include "Storage.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

namespace {
long long toUnixSeconds(std::chrono::system_clock::time_point time)
{
    return std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch()).count();
}

std::chrono::system_clock::time_point fromUnixSeconds(long long value)
{
    return std::chrono::system_clock::time_point{std::chrono::seconds{value}};
}
}

Storage::Storage(std::string directory)
    : directory_(std::move(directory))
{
}

std::vector<Task> Storage::loadTasks() const
{
    std::vector<Task> tasks;
    std::ifstream input(tasksPath());
    std::string line;

    while (std::getline(input, line)) {
        std::stringstream row(line);
        std::string id;
        std::string title;
        std::string description;
        std::string status;

        if (std::getline(row, id, '|') &&
            std::getline(row, title, '|') &&
            std::getline(row, description, '|') &&
            std::getline(row, status)) {
            tasks.push_back(Task{std::stoi(id), title, description, taskStatusFromString(status)});
        }
    }

    return tasks;
}

std::vector<TimeSession> Storage::loadSessions() const
{
    std::vector<TimeSession> sessions;
    std::ifstream input(sessionsPath());
    std::string line;

    while (std::getline(input, line)) {
        std::stringstream row(line);
        std::string id;
        std::string taskId;
        std::string startTime;
        std::string endTime;
        std::string active;

        if (std::getline(row, id, '|') &&
            std::getline(row, taskId, '|') &&
            std::getline(row, startTime, '|') &&
            std::getline(row, endTime, '|') &&
            std::getline(row, active)) {
            sessions.push_back(TimeSession{
                std::stoi(id),
                std::stoi(taskId),
                fromUnixSeconds(std::stoll(startTime)),
                fromUnixSeconds(std::stoll(endTime)),
                active == "1"
            });
        }
    }

    return sessions;
}

void Storage::saveTasks(const std::vector<Task>& tasks) const
{
    std::filesystem::create_directories(directory_);
    std::ofstream output(tasksPath());
    for (const auto& task : tasks) {
        output << task.id << '|'
               << task.title << '|'
               << task.description << '|'
               << toString(task.status) << '\n';
    }
}

void Storage::saveSessions(const std::vector<TimeSession>& sessions) const
{
    std::filesystem::create_directories(directory_);
    std::ofstream output(sessionsPath());
    for (const auto& session : sessions) {
        output << session.id << '|'
               << session.taskId << '|'
               << toUnixSeconds(session.startTime) << '|'
               << toUnixSeconds(session.endTime) << '|'
               << (session.active ? "1" : "0") << '\n';
    }
}

std::string Storage::tasksPath() const
{
    return directory_ + "/tasks.csv";
}

std::string Storage::sessionsPath() const
{
    return directory_ + "/sessions.csv";
}
