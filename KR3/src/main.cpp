#include "ConsoleUI.h"

int main()
{
    TaskManager taskManager;
    Storage storage("data");
    taskManager.replaceTasks(storage.loadTasks());

    TimeTracker timeTracker(taskManager);
    timeTracker.replaceSessions(storage.loadSessions());

    ConsoleUI ui(taskManager, timeTracker, storage);
    ui.run();
    return 0;
}
