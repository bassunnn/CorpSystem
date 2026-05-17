#include "Task.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("TaskStatus is converted to string")
{
    REQUIRE(toString(TaskStatus::Active) == "active");
    REQUIRE(toString(TaskStatus::Archived) == "archived");
}

TEST_CASE("TaskStatus is parsed from known strings")
{
    REQUIRE(taskStatusFromString("active") == TaskStatus::Active);
    REQUIRE(taskStatusFromString("archived") == TaskStatus::Archived);
}

TEST_CASE("Unknown task status defaults to active")
{
    REQUIRE(taskStatusFromString("") == TaskStatus::Active);
    REQUIRE(taskStatusFromString("deleted") == TaskStatus::Active);
    REQUIRE(taskStatusFromString("ACTIVE") == TaskStatus::Active);
}

TEST_CASE("Task can store required fields")
{
    Task task{7, "Course work", "Prepare report", TaskStatus::Active};

    REQUIRE(task.id == 7);
    REQUIRE(task.title == "Course work");
    REQUIRE(task.description == "Prepare report");
    REQUIRE(task.status == TaskStatus::Active);
}

TEST_CASE("Task status can be changed")
{
    Task task{1, "Task", "Description", TaskStatus::Active};
    task.status = TaskStatus::Archived;

    REQUIRE(task.status == TaskStatus::Archived);
    REQUIRE(toString(task.status) == "archived");
}
