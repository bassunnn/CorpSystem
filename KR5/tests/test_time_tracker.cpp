#include "TimeTracker.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("TimeTracker starts session for active task")
{
    TaskManager manager;
    const int id = manager.addTask("Task", "");
    TimeTracker tracker(manager);

    REQUIRE(tracker.start(id));
    REQUIRE(tracker.activeSession().has_value());
}

TEST_CASE("TimeTracker refuses missing task")
{
    TaskManager manager;
    TimeTracker tracker(manager);

    REQUIRE_FALSE(tracker.start(100));
    REQUIRE_FALSE(tracker.activeSession().has_value());
}

TEST_CASE("TimeTracker refuses archived task")
{
    TaskManager manager;
    const int id = manager.addTask("Task", "");
    manager.archiveTask(id);
    TimeTracker tracker(manager);

    REQUIRE_FALSE(tracker.start(id));
}

TEST_CASE("TimeTracker allows only one active session")
{
    TaskManager manager;
    const int first = manager.addTask("First", "");
    const int second = manager.addTask("Second", "");
    TimeTracker tracker(manager);

    REQUIRE(tracker.start(first));
    REQUIRE_FALSE(tracker.start(second));
    REQUIRE(tracker.sessions().size() == 1);
}

TEST_CASE("TimeTracker stops active session")
{
    TaskManager manager;
    const int id = manager.addTask("Task", "");
    TimeTracker tracker(manager);
    tracker.start(id);

    const auto stopped = tracker.stop();

    REQUIRE(stopped.has_value());
    REQUIRE_FALSE(stopped->active);
    REQUIRE_FALSE(tracker.activeSession().has_value());
}

TEST_CASE("TimeTracker returns empty optional when stopping without active session")
{
    TaskManager manager;
    TimeTracker tracker(manager);

    REQUIRE_FALSE(tracker.stop().has_value());
}

TEST_CASE("TimeTracker replaces loaded sessions and continues id sequence")
{
    TaskManager manager;
    const int taskId = manager.addTask("Task", "");
    TimeTracker tracker(manager);
    tracker.replaceSessions({TimeSession{5, taskId, {}, {}, false}});

    REQUIRE(tracker.start(taskId));
    REQUIRE(tracker.activeSession()->id == 6);
}
