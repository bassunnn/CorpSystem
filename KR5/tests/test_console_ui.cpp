#include "ConsoleUI.h"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <iostream>
#include <sstream>

namespace {
struct ConsoleHarness {
    std::istringstream input;
    std::ostringstream output;
    std::streambuf* oldInput{};
    std::streambuf* oldOutput{};

    explicit ConsoleHarness(const std::string& commands)
        : input(commands)
    {
        oldInput = std::cin.rdbuf(input.rdbuf());
        oldOutput = std::cout.rdbuf(output.rdbuf());
    }

    ~ConsoleHarness()
    {
        std::cin.rdbuf(oldInput);
        std::cout.rdbuf(oldOutput);
    }
};

std::string tempDirectory(const std::string& name)
{
    const auto path = std::filesystem::temp_directory_path() / ("time_tracker_console_" + name);
    std::filesystem::remove_all(path);
    return path.string();
}
}

TEST_CASE("ConsoleUI exits and saves empty data")
{
    TaskManager manager;
    TimeTracker tracker(manager);
    Storage storage(tempDirectory("exit"));
    ConsoleUI ui(manager, tracker, storage);
    ConsoleHarness console("0\n");

    ui.run();

    REQUIRE(console.output.str().find("Data saved.") != std::string::npos);
}

TEST_CASE("ConsoleUI adds task through menu")
{
    TaskManager manager;
    TimeTracker tracker(manager);
    Storage storage(tempDirectory("add"));
    ConsoleUI ui(manager, tracker, storage);
    ConsoleHarness console("1\nStudy\nRead docs\n0\n");

    ui.run();

    REQUIRE(manager.tasks().size() == 1);
    REQUIRE(manager.tasks()[0].title == "Study");
}

TEST_CASE("ConsoleUI lists tasks")
{
    TaskManager manager;
    manager.addTask("Visible", "Description");
    TimeTracker tracker(manager);
    Storage storage(tempDirectory("list"));
    ConsoleUI ui(manager, tracker, storage);
    ConsoleHarness console("2\n0\n");

    ui.run();

    REQUIRE(console.output.str().find("Visible") != std::string::npos);
}

TEST_CASE("ConsoleUI archives task")
{
    TaskManager manager;
    const int id = manager.addTask("Old", "");
    TimeTracker tracker(manager);
    Storage storage(tempDirectory("archive"));
    ConsoleUI ui(manager, tracker, storage);
    ConsoleHarness console("3\n" + std::to_string(id) + "\n0\n");

    ui.run();

    REQUIRE(manager.findTask(id)->status == TaskStatus::Archived);
}

TEST_CASE("ConsoleUI deletes task and related sessions")
{
    TaskManager manager;
    const int id = manager.addTask("Temporary", "");
    TimeTracker tracker(manager);
    tracker.start(id);
    tracker.stop();
    Storage storage(tempDirectory("delete"));
    ConsoleUI ui(manager, tracker, storage);
    ConsoleHarness console("8\n" + std::to_string(id) + "\n0\n");

    ui.run();

    REQUIRE_FALSE(manager.findTask(id).has_value());
    REQUIRE(tracker.sessions().empty());
    REQUIRE(console.output.str().find("Task deleted.") != std::string::npos);
}

TEST_CASE("ConsoleUI starts and stops timer")
{
    TaskManager manager;
    const int id = manager.addTask("Timed", "");
    TimeTracker tracker(manager);
    Storage storage(tempDirectory("timer"));
    ConsoleUI ui(manager, tracker, storage);
    ConsoleHarness console("4\n" + std::to_string(id) + "\n5\n0\n");

    ui.run();

    REQUIRE(tracker.sessions().size() == 1);
    REQUIRE_FALSE(tracker.sessions()[0].active);
}

TEST_CASE("ConsoleUI handles unknown command")
{
    TaskManager manager;
    TimeTracker tracker(manager);
    Storage storage(tempDirectory("unknown"));
    ConsoleUI ui(manager, tracker, storage);
    ConsoleHarness console("99\n0\n");

    ui.run();

    REQUIRE(console.output.str().find("Unknown command.") != std::string::npos);
}

TEST_CASE("ConsoleUI prints report")
{
    TaskManager manager;
    const int id = manager.addTask("Report task", "");
    TimeTracker tracker(manager);
    tracker.start(id);
    tracker.stop();
    Storage storage(tempDirectory("report"));
    ConsoleUI ui(manager, tracker, storage);
    ConsoleHarness console("7\n0\n");

    ui.run();

    REQUIRE(console.output.str().find("Report task") != std::string::npos);
}
