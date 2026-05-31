#ifndef KR3_TIME_TRACKER_H
#define KR3_TIME_TRACKER_H

#include "TaskManager.h"
#include "TimeSession.h"

#include <optional>
#include <vector>

class TimeTracker {
public:
    explicit TimeTracker(const TaskManager& taskManager);

    bool start(int taskId);
    std::optional<TimeSession> stop();
    const std::vector<TimeSession>& sessions() const;
    std::optional<TimeSession> activeSession() const;
    void replaceSessions(std::vector<TimeSession> loadedSessions);
    int deleteSessionsForTask(int taskId);

private:
    int nextId() const;

    const TaskManager& taskManager_;
    std::vector<TimeSession> sessions_;
};

#endif
