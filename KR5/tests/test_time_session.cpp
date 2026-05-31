#include "TimeSession.h"

#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <thread>

TEST_CASE("Finished session returns exact duration")
{
    TimeSession session{1, 2, std::chrono::system_clock::time_point{std::chrono::seconds{10}}, std::chrono::system_clock::time_point{std::chrono::seconds{75}}, false};

    REQUIRE(session.durationSeconds() == 65);
}

TEST_CASE("Zero-length finished session returns zero")
{
    const auto time = std::chrono::system_clock::time_point{std::chrono::seconds{42}};
    TimeSession session{1, 2, time, time, false};

    REQUIRE(session.durationSeconds() == 0);
}

TEST_CASE("Active session duration is non-negative")
{
    TimeSession session{1, 2, std::chrono::system_clock::now(), {}, true};

    REQUIRE(session.durationSeconds() >= 0);
}

TEST_CASE("Active session duration grows over time")
{
    TimeSession session{1, 2, std::chrono::system_clock::now() - std::chrono::seconds{2}, {}, true};
    const auto first = session.durationSeconds();
    std::this_thread::sleep_for(std::chrono::milliseconds{20});

    REQUIRE(session.durationSeconds() >= first);
}

TEST_CASE("Session stores identifiers and active flag")
{
    TimeSession session{11, 5, {}, {}, true};

    REQUIRE(session.id == 11);
    REQUIRE(session.taskId == 5);
    REQUIRE(session.active);
}
