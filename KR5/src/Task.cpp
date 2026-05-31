#include "Task.h"

std::string toString(TaskStatus status)
{
    return status == TaskStatus::Active ? "active" : "archived";
}

TaskStatus taskStatusFromString(const std::string& value)
{
    return value == "archived" ? TaskStatus::Archived : TaskStatus::Active;
}
