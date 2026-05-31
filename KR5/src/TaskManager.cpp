#include "TaskManager.h"

#include <algorithm>
#include <utility>

int TaskManager::addTask(const std::string& title, const std::string& description)
{
    const int id = nextId();
    tasks_.push_back(Task{id, title, description, TaskStatus::Active});
    return id;
}

bool TaskManager::archiveTask(int taskId)
{
    auto task = std::find_if(tasks_.begin(), tasks_.end(), [taskId](const Task& item) {
        return item.id == taskId;
    });
    if (task == tasks_.end()) {
        return false;
    }

    task->status = TaskStatus::Archived;
    return true;
}

bool TaskManager::deleteTask(int taskId)
{
    const auto oldSize = tasks_.size();
    tasks_.erase(
        std::remove_if(tasks_.begin(), tasks_.end(), [taskId](const Task& item) {
            return item.id == taskId;
        }),
        tasks_.end());
    return tasks_.size() != oldSize;
}

const std::vector<Task>& TaskManager::tasks() const
{
    return tasks_;
}

std::optional<Task> TaskManager::findTask(int taskId) const
{
    auto task = std::find_if(tasks_.begin(), tasks_.end(), [taskId](const Task& item) {
        return item.id == taskId;
    });
    if (task == tasks_.end()) {
        return std::nullopt;
    }

    return *task;
}

void TaskManager::replaceTasks(std::vector<Task> loadedTasks)
{
    tasks_ = std::move(loadedTasks);
}

int TaskManager::nextId() const
{
    int maxId = 0;
    for (const auto& task : tasks_) {
        maxId = std::max(maxId, task.id);
    }
    return maxId + 1;
}
