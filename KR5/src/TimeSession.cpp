#include "TimeSession.h"

long long TimeSession::durationSeconds() const
{
    const auto end = active ? std::chrono::system_clock::now() : endTime;
    return std::chrono::duration_cast<std::chrono::seconds>(end - startTime).count();
}
