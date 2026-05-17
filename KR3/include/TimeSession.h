#ifndef KR3_TIME_SESSION_H
#define KR3_TIME_SESSION_H

#include <chrono>

struct TimeSession {
    int id{};
    int taskId{};
    std::chrono::system_clock::time_point startTime{};
    std::chrono::system_clock::time_point endTime{};
    bool active{false};

    long long durationSeconds() const;
};

#endif
