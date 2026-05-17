#include "ReportService.h"

#include <iomanip>
#include <iostream>

std::map<int, long long> ReportService::totalSecondsByTask(const std::vector<TimeSession>& sessions) const
{
    std::map<int, long long> totals;
    for (const auto& session : sessions) {
        if (!session.active) {
            totals[session.taskId] += session.durationSeconds();
        }
    }
    return totals;
}

void ReportService::printReport(const std::vector<Task>& tasks, const std::vector<TimeSession>& sessions) const
{
    const auto totals = totalSecondsByTask(sessions);
    std::cout << "\nReport\n";
    std::cout << "------\n";

    for (const auto& task : tasks) {
        const long long seconds = totals.count(task.id) == 0 ? 0 : totals.at(task.id);
        std::cout << '#' << task.id << " " << task.title << ": "
                  << seconds / 3600 << "h "
                  << (seconds % 3600) / 60 << "m "
                  << seconds % 60 << "s\n";
    }
}
