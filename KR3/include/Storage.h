#ifndef KR3_STORAGE_H
#define KR3_STORAGE_H

#include "Task.h"
#include "TimeSession.h"

#include <string>
#include <vector>

class Storage {
public:
    explicit Storage(std::string directory);

    std::vector<Task> loadTasks() const;
    std::vector<TimeSession> loadSessions() const;
    void saveTasks(const std::vector<Task>& tasks) const;
    void saveSessions(const std::vector<TimeSession>& sessions) const;

private:
    std::string tasksPath() const;
    std::string sessionsPath() const;

    std::string directory_;
};

#endif
