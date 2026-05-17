#ifndef KR3_CONSOLE_UI_H
#define KR3_CONSOLE_UI_H

#include "ReportService.h"
#include "Storage.h"
#include "TaskManager.h"
#include "TimeTracker.h"

class ConsoleUI {
public:
    ConsoleUI(TaskManager& taskManager, TimeTracker& timeTracker, Storage& storage);

    void run();

private:
    void addTask();
    void listTasks() const;
    void archiveTask();
    void startTimer();
    void stopTimer();
    void showHistory() const;
    void save() const;

    TaskManager& taskManager_;
    TimeTracker& timeTracker_;
    Storage& storage_;
    ReportService reportService_;
};

#endif
