#include "TaskManager.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("TaskManager adds task and assigns id")
{
    TaskManager manager;

    REQUIRE(manager.addTask("Study", "Read chapter") == 1);
    REQUIRE(manager.tasks().size() == 1);
    REQUIRE(manager.tasks().front().title == "Study");
}

TEST_CASE("TaskManager increments ids")
{
    TaskManager manager;

    REQUIRE(manager.addTask("One", "") == 1);
    REQUIRE(manager.addTask("Two", "") == 2);
    REQUIRE(manager.addTask("Three", "") == 3);
}

TEST_CASE("TaskManager finds existing task")
{
    TaskManager manager;
    const int id = manager.addTask("Existing", "Description");

    const auto task = manager.findTask(id);

    REQUIRE(task.has_value());
    REQUIRE(task->title == "Existing");
}

TEST_CASE("TaskManager returns empty optional for missing task")
{
    TaskManager manager;

    REQUIRE_FALSE(manager.findTask(999).has_value());
}

TEST_CASE("TaskManager archives existing task")
{
    TaskManager manager;
    const int id = manager.addTask("Archive me", "");

    REQUIRE(manager.archiveTask(id));
    REQUIRE(manager.findTask(id)->status == TaskStatus::Archived);
}

TEST_CASE("TaskManager refuses to archive missing task")
{
    TaskManager manager;

    REQUIRE_FALSE(manager.archiveTask(45));
}

TEST_CASE("TaskManager deletes existing task")
{
    TaskManager manager;
    const int first = manager.addTask("First", "");
    const int second = manager.addTask("Second", "");

    REQUIRE(manager.deleteTask(first));
    REQUIRE_FALSE(manager.findTask(first).has_value());
    REQUIRE(manager.findTask(second).has_value());
    REQUIRE(manager.tasks().size() == 1);
}

TEST_CASE("TaskManager refuses to delete missing task")
{
    TaskManager manager;

    REQUIRE_FALSE(manager.deleteTask(45));
}

TEST_CASE("TaskManager replaces loaded tasks and continues id sequence")
{
    TaskManager manager;
    manager.replaceTasks({Task{3, "Loaded", "", TaskStatus::Active}});

    REQUIRE(manager.tasks().size() == 1);
    REQUIRE(manager.addTask("Next", "") == 4);
}
