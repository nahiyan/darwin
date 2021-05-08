#ifndef _HELPERS_TIME_H_
#define _HELPERS_TIME_H_

#include <chrono>

class TimeHelper
{
public:
    // Milliseconds
    static long long now()
    {
        typedef std::chrono::high_resolution_clock myclock;
        return std::chrono::duration_cast<std::chrono::milliseconds>(myclock::now().time_since_epoch()).count();
    }
    // Nanoseconds
    static long long nowNanoSeconds()
    {
        typedef std::chrono::high_resolution_clock myclock;
        return std::chrono::duration_cast<std::chrono::nanoseconds>(myclock::now().time_since_epoch()).count();
    }

    static long long diff(long long time)
    {
        return TimeHelper::now() - time;
    }
};

#endif // _HELPERS_TIME_H_
