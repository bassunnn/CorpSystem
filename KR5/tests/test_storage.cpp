#include "Storage.h"

#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <filesystem>
#include <string>

namespace {
std::string tempDirectory(const std::string& name)
{
    const auto path = std::filesystem::temp_directory_path() / ("time_tracker_tests_" + name);
    std::filesystem::remove_all(path);
    return path.string();
}
}

TEST_CASE("Storage returns empty vectors when files are absent")
{
    const auto dir = tempDirectory("empty");
    Storage storage(dir);

    REQUIRE(storage.loadTasks().empty());
    REQUIRE(storage.loadSessions().empty());
}

TEST_CASE("Storage saves and loads tasks")
{
    const auto dir = tempDirectory("tasks");
    Storage storage(dir);
    storage.saveTasks({Task{1, "Task", "Description", TaskStatus::Active}});

    const auto tasks = storage.loadTasks();

    REQUIRE(tasks.size() == 1);
    REQUIRE(tasks[0].id == 1);
    REQUIRE(tasks[0].title == "Task");
    REQUIRE(tasks[0].description == "Description");
    REQUIRE(tasks[0].status == TaskStatus::Active);
}

TEST_CASE("Storage saves and loads archived status")
{
    const auto dir = tempDirectory("archived");
    Storage storage(dir);
    storage.saveTasks({Task{2, "Old", "", TaskStatus::Archived}});

    REQUIRE(storage.loadTasks()[0].status == TaskStatus::Archived);
}

TEST_CASE("Storage saves and loads multiple tasks")
{
    const auto dir = tempDirectory("many_tasks");
    Storage storage(dir);
    storage.saveTasks({
        Task{1, "One", "", TaskStatus::Active},
        Task{2, "Two", "", TaskStatus::Archived}
    });

    REQUIRE(storage.loadTasks().size() == 2);
}

TEST_CASE("Storage saves and loads sessions")
{
    const auto dir = tempDirectory("sessions");
    Storage storage(dir);
    const auto start = std::chrono::system_clock::time_point{std::chrono::seconds{100}};
    const auto end = std::chrono::system_clock::time_point{std::chrono::seconds{160}};
    storage.saveSessions({TimeSession{1, 7, start, end, false}});

    const auto sessions = storage.loadSessions();

    REQUIRE(sessions.size() == 1);
    REQUIRE(sessions[0].id == 1);
    REQUIRE(sessions[0].taskId == 7);
    REQUIRE(sessions[0].durationSeconds() == 60);
}

TEST_CASE("Storage saves active session flag")
{
    const auto dir = tempDirectory("active_session");
    Storage storage(dir);
    storage.saveSessions({TimeSession{1, 7, std::chrono::system_clock::now(), {}, true}});

    REQUIRE(storage.loadSessions()[0].active);
}

TEST_CASE("Storage overwrites previous data")
{
    const auto dir = tempDirectory("overwrite");
    Storage storage(dir);
    storage.saveTasks({Task{1, "Old", "", TaskStatus::Active}});
    storage.saveTasks({Task{2, "New", "", TaskStatus::Active}});

    const auto tasks = storage.loadTasks();

    REQUIRE(tasks.size() == 1);
    REQUIRE(tasks[0].id == 2);
}
