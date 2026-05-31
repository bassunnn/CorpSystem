#ifndef KR3_TASK_MANAGER_H
#define KR3_TASK_MANAGER_H

#include "Task.h"

#include <optional>
#include <string>
#include <vector>

class TaskManager {
public:
    int addTask(const std::string& title, const std::string& description);
    bool archiveTask(int taskId);
    const std::vector<Task>& tasks() const;
    std::optional<Task> findTask(int taskId) const;
    void replaceTasks(std::vector<Task> loadedTasks);

private:
    int nextId() const;

    std::vector<Task> tasks_;
};

#endif
