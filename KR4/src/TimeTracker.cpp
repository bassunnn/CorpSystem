#include "TimeTracker.h"

#include <algorithm>
#include <chrono>
#include <utility>

TimeTracker::TimeTracker(const TaskManager& taskManager)
    : taskManager_(taskManager)
{
}

bool TimeTracker::start(int taskId)
{
    if (activeSession().has_value()) {
        return false;
    }

    const auto task = taskManager_.findTask(taskId);
    if (!task.has_value() || task->status != TaskStatus::Active) {
        return false;
    }

    sessions_.push_back(TimeSession{
        nextId(),
        taskId,
        std::chrono::system_clock::now(),
        {},
        true
    });
    return true;
}

std::optional<TimeSession> TimeTracker::stop()
{
    auto session = std::find_if(sessions_.begin(), sessions_.end(), [](const TimeSession& item) {
        return item.active;
    });
    if (session == sessions_.end()) {
        return std::nullopt;
    }

    session->endTime = std::chrono::system_clock::now();
    session->active = false;
    return *session;
}

const std::vector<TimeSession>& TimeTracker::sessions() const
{
    return sessions_;
}

std::optional<TimeSession> TimeTracker::activeSession() const
{
    auto session = std::find_if(sessions_.begin(), sessions_.end(), [](const TimeSession& item) {
        return item.active;
    });
    if (session == sessions_.end()) {
        return std::nullopt;
    }

    return *session;
}

void TimeTracker::replaceSessions(std::vector<TimeSession> loadedSessions)
{
    sessions_ = std::move(loadedSessions);
}

int TimeTracker::nextId() const
{
    int maxId = 0;
    for (const auto& session : sessions_) {
        maxId = std::max(maxId, session.id);
    }
    return maxId + 1;
}
