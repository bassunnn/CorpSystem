#ifndef KR3_REPORT_SERVICE_H
#define KR3_REPORT_SERVICE_H

#include "Task.h"
#include "TimeSession.h"

#include <map>
#include <vector>

class ReportService {
public:
    std::map<int, long long> totalSecondsByTask(const std::vector<TimeSession>& sessions) const;
    void printReport(const std::vector<Task>& tasks, const std::vector<TimeSession>& sessions) const;
};

#endif
