#include "Storage.h"

#include <chrono>
#include <filesystem>
#include <iostream>

int main()
{
    const auto directory = std::filesystem::temp_directory_path() / "time_tracker_scenario_persistence";
    std::filesystem::remove_all(directory);

    Storage storage(directory.string());
    storage.saveTasks({Task{1, "Persisted task", "Saved to disk", TaskStatus::Active}});
    storage.saveSessions({
        TimeSession{
            1,
            1,
            std::chrono::system_clock::time_point{std::chrono::seconds{10}},
            std::chrono::system_clock::time_point{std::chrono::seconds{40}},
            false
        }
    });

    const auto tasks = storage.loadTasks();
    const auto sessions = storage.loadSessions();
    if (tasks.size() != 1 || sessions.size() != 1 || sessions[0].durationSeconds() != 30) {
        std::cerr << "Persistence scenario failed\n";
        return 1;
    }

    std::cout << "Persistence scenario passed\n";
    return 0;
}
