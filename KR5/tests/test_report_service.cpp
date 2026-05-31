#include "ReportService.h"

#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <iostream>
#include <sstream>

namespace {
TimeSession finishedSession(int id, int taskId, long long start, long long end)
{
    return TimeSession{
        id,
        taskId,
        std::chrono::system_clock::time_point{std::chrono::seconds{start}},
        std::chrono::system_clock::time_point{std::chrono::seconds{end}},
        false
    };
}
}

TEST_CASE("ReportService returns empty totals for empty input")
{
    ReportService service;

    REQUIRE(service.totalSecondsByTask({}).empty());
}

TEST_CASE("ReportService sums one task")
{
    ReportService service;
    const auto totals = service.totalSecondsByTask({finishedSession(1, 10, 0, 60)});

    REQUIRE(totals.at(10) == 60);
}

TEST_CASE("ReportService sums several sessions for one task")
{
    ReportService service;
    const auto totals = service.totalSecondsByTask({
        finishedSession(1, 10, 0, 60),
        finishedSession(2, 10, 100, 160)
    });

    REQUIRE(totals.at(10) == 120);
}

TEST_CASE("ReportService separates tasks")
{
    ReportService service;
    const auto totals = service.totalSecondsByTask({
        finishedSession(1, 10, 0, 60),
        finishedSession(2, 20, 0, 30)
    });

    REQUIRE(totals.at(10) == 60);
    REQUIRE(totals.at(20) == 30);
}

TEST_CASE("ReportService ignores active sessions")
{
    ReportService service;
    TimeSession active{1, 10, std::chrono::system_clock::now(), {}, true};

    REQUIRE(service.totalSecondsByTask({active}).empty());
}

TEST_CASE("ReportService prints task names and time")
{
    ReportService service;
    std::ostringstream output;
    auto* oldBuffer = std::cout.rdbuf(output.rdbuf());

    service.printReport({Task{10, "Study", "", TaskStatus::Active}}, {finishedSession(1, 10, 0, 3661)});

    std::cout.rdbuf(oldBuffer);
    REQUIRE(output.str().find("Study") != std::string::npos);
    REQUIRE(output.str().find("1h 1m 1s") != std::string::npos);
}

TEST_CASE("ReportService prints zero for tasks without sessions")
{
    ReportService service;
    std::ostringstream output;
    auto* oldBuffer = std::cout.rdbuf(output.rdbuf());

    service.printReport({Task{10, "Empty", "", TaskStatus::Active}}, {});

    std::cout.rdbuf(oldBuffer);
    REQUIRE(output.str().find("0h 0m 0s") != std::string::npos);
}
