#include "ReportService.h"

#include <chrono>
#include <iostream>

int main()
{
    ReportService reportService;
    const std::vector<Task> tasks = {
        Task{1, "Development", "", TaskStatus::Active},
        Task{2, "Documentation", "", TaskStatus::Active}
    };
    const std::vector<TimeSession> sessions = {
        TimeSession{1, 1, std::chrono::system_clock::time_point{std::chrono::seconds{0}}, std::chrono::system_clock::time_point{std::chrono::seconds{3600}}, false},
        TimeSession{2, 2, std::chrono::system_clock::time_point{std::chrono::seconds{0}}, std::chrono::system_clock::time_point{std::chrono::seconds{900}}, false}
    };

    const auto totals = reportService.totalSecondsByTask(sessions);
    if (totals.at(1) != 3600 || totals.at(2) != 900) {
        std::cerr << "Reporting scenario failed\n";
        return 1;
    }

    reportService.printReport(tasks, sessions);
    std::cout << "Reporting scenario passed\n";
    return 0;
}
