#ifndef KR3_TASK_H
#define KR3_TASK_H

#include <string>

enum class TaskStatus {
    Active,
    Archived
};

struct Task {
    int id{};
    std::string title;
    std::string description;
    TaskStatus status{TaskStatus::Active};
};

std::string toString(TaskStatus status);
TaskStatus taskStatusFromString(const std::string& value);

#endif
