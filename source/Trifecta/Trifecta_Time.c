#include "Trifecta_Time.h"

#include <stdbool.h>

#include "PGUtil.h"

#ifdef __APPLE__

#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

/* 
 * Returns TimePoint representing the specified number
 * of nanoseconds
 */
TimePoint makeTimeNano(uint64_t nanoseconds){
    TimePoint toRet = {0};
    return addTimeNano(toRet, nanoseconds);
}

/* Returns the current time */
TimePoint getCurrentTime(){
    /* 
     * OSX clock_gettime resolution is 1000ns; for
     * more precision use mach_absolute_time 
     */

    /* get absolute time */
    uint64_t time = mach_absolute_time();
    /* convert to Nanoseconds */
    Nanoseconds timeNano = AbsoluteToNanoseconds(
        *(AbsoluteTime*)&time
    );
    /* convert to TimePoint */
    TimePoint toRet = {0};
    toRet = addTimeNano(toRet, *(uint64_t*)&timeNano);
    return toRet;
}

/* 
 * Returns the result of adding the specified amount
 * of nanoseconds to the specified TimePoint
 */
TimePoint addTimeNano(
    TimePoint timePoint,
    uint64_t timeNano
){
    unsigned int secondsToAdd = timeNano / _oneBillion;
    uint64_t nanosecondsToAdd = timeNano % _oneBillion;
    timePoint.tv_sec += secondsToAdd;
    timePoint.tv_nsec += nanosecondsToAdd;

    /* carry if needed*/
    if(timePoint.tv_nsec >= _oneBillion){
        ++(timePoint.tv_sec);
        timePoint.tv_nsec -= _oneBillion;
    }

    return timePoint;
}

/* 
 * Returns a negative value if the first TimePoint
 * is before the second, a positive value if the
 * first TimePoint is after the second, or 0 if
 * the two TimePoints are the same
 */
int timePointCompare(TimePoint left, TimePoint right){
    if(left.tv_sec < right.tv_sec){
        return -1;
    }
    if(left.tv_sec > right.tv_sec){
        return 1;
    }
    if(left.tv_nsec < right.tv_nsec){
        return -1;
    }
    if(left.tv_nsec > right.tv_nsec){
        return 1;
    }
    return 0;
}

/*
 * Returns the difference between the two specified
 * TimePoints in nanoseconds
 */
int64_t timePointDiffNano(
    TimePoint left,
    TimePoint right
){
    int64_t secDiff = left.tv_sec - right.tv_sec;
    int64_t nanoDiff = left.tv_nsec - right.tv_nsec;

    int64_t diff = (secDiff * _oneBillion) + nanoDiff;
    return diff;
}

/* Sleeps until the specified TimePoint */
void sleepUntil(TimePoint timePoint){
    /* initialize variables */
    TimePoint currentTime = {0};
    int64_t timeDiff = 0;
    TimePoint waitTime = {0};

    /* loop to account for interruptions */
    while(true){
        currentTime = getCurrentTime();
        timeDiff = timePointDiffNano(
            timePoint,
            currentTime
        );
        /* return if we have reached timePoint */
        if(timeDiff <= 0){
            return;
        }

        /* otherwise nanosleep() */
        waitTime.tv_sec = timeDiff / _oneBillion;
        waitTime.tv_nsec = timeDiff % _oneBillion;
        int retval = nanosleep(&waitTime, NULL);

        /* if nanosleep completed, return */
        if(retval == 0){
            return;
        }
        /* otherwise loop back to start */
    }
}

#endif /* end __APPLE__ */

#ifdef WIN32

/* 
 * Returns TimePoint representing the specified number
 * of nanoseconds
 */
TimePoint makeTimeNano(uint64_t nanoseconds){
    return (TimePoint)nanoseconds;
}

/* Returns the current time */
TimePoint getCurrentTime(){
    /* frequency in ticks per second */
    LARGE_INTEGER frequency = {0};
    LARGE_INTEGER count = {0};
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&count);

    count.QuadPart *= _oneBillion;
    count.QuadPart /= frequency.QuadPart;
    return count.QuadPart;
}

/* 
 * Returns the result of adding the specified amount
 * of nanoseconds to the specified TimePoint
 */
TimePoint addTimeNano(
    TimePoint timePoint,
    uint64_t timeNano
){
    return timePoint + (TimePoint)timeNano;
}

/* 
 * Returns a negative value if the first TimePoint
 * is before the second, a positive value if the
 * first TimePoint is after the second, or 0 if
 * the two TimePoints are the same
 */
int timePointCompare(TimePoint left, TimePoint right){
    if(left < right){
        return -1;
    }
    if(left > right){
        return 1;
    }
    return 0;
}

/*
 * Returns the difference between the two specified
 * TimePoints in nanoseconds
 */
int64_t timePointDiffNano(
    TimePoint left,
    TimePoint right
){
    return left - right;
}

/* Sleeps until the specified TimePoint */
void sleepUntil(TimePoint timePoint){
    /* initialize variables */

    TimePoint currentTime = getCurrentTime();
    int64_t timeDiff = timePointDiffNano(
        timePoint,
        currentTime
    );
    /* return if we have reached timePoint */
    if(timeDiff <= 0){
        return;
    }

    HANDLE timerHandle = CreateWaitableTimer(
        NULL,
        true,
        NULL)
    ;
    if(!timerHandle){
        pgError(
            "failed to open timer handle; "
            SRC_LOCATION
        );
    }

    LARGE_INTEGER timeToWait = {0};
    timeToWait.QuadPart = -timeDiff / 100; /* 100ns */

    if(!SetWaitableTimer(
        timerHandle,
        &timeToWait,
        0,
        NULL,
        NULL,
        false
    )){
        pgError("error setting timer; " SRC_LOCATION);
    }

    WaitForSingleObject(timerHandle, INFINITE);

    CloseHandle(timerHandle);
}

#endif /* end WIN32 */

#ifdef __linux__

#include <linux/time.h>
#include <time.h>

/* 
 * Returns TimePoint representing the specified number
 * of nanoseconds
 */
TimePoint makeTimeNano(uint64_t nanoseconds){
    TimePoint toRet = {0};
    return addTimeNano(toRet, nanoseconds);
}

/* Returns the current time */
TimePoint getCurrentTime(){
    TimePoint toRet = {0};
    int retval = clock_gettime(
        CLOCK_MONOTONIC_RAW,
        &toRet
    );
    if(retval == 0){
        return toRet;
    }

    retval = clock_gettime(
        CLOCK_MONOTONIC,
        &toRet
    );
    if(retval == 0){
        return toRet;
    }

    retval = clock_gettime(
        CLOCK_REALTIME,
        &toRet
    );
    if(retval == 0){
        return toRet;
    }

    pgError(
        "failed to get time from monotonic raw, "
        "monotonic, and realtime; " SRC_LOCATION
    );
    return toRet;
}

/* 
 * Returns the result of adding the specified amount
 * of nanoseconds to the specified TimePoint
 */
TimePoint addTimeNano(
    TimePoint timePoint,
    uint64_t timeNano
){
    unsigned int secondsToAdd = timeNano / _oneBillion;
    uint64_t nanosecondsToAdd = timeNano % _oneBillion;
    timePoint.tv_sec += secondsToAdd;
    timePoint.tv_nsec += nanosecondsToAdd;

    /* carry if needed*/
    if(timePoint.tv_nsec >= _oneBillion){
        ++(timePoint.tv_sec);
        timePoint.tv_nsec -= _oneBillion;
    }

    return timePoint;
}

/* 
 * Returns a negative value if the first TimePoint
 * is before the second, a positive value if the
 * first TimePoint is after the second, or 0 if
 * the two TimePoints are the same
 */
int timePointCompare(TimePoint left, TimePoint right){
    if(left.tv_sec < right.tv_sec){
        return -1;
    }
    if(left.tv_sec > right.tv_sec){
        return 1;
    }
    if(left.tv_nsec < right.tv_nsec){
        return -1;
    }
    if(left.tv_nsec > right.tv_nsec){
        return 1;
    }
    return 0;
}

/*
 * Returns the difference between the two specified
 * TimePoints in nanoseconds
 */
int64_t timePointDiffNano(
    TimePoint left,
    TimePoint right
){
    int64_t secDiff = left.tv_sec - right.tv_sec;
    int64_t nanoDiff = left.tv_nsec - right.tv_nsec;

    int64_t diff = (secDiff * _oneBillion) + nanoDiff;
    return diff;
}

/* Sleeps until the specified TimePoint */
void sleepUntil(TimePoint timePoint){
    /* initialize variables */
    TimePoint currentTime = {0};
    int64_t timeDiff = 0;
    TimePoint waitTime = {0};

    /* loop to account for interruptions */
    while(true){
        currentTime = getCurrentTime();
        timeDiff = timePointDiffNano(
            timePoint,
            currentTime
        );
        /* return if we have reached timePoint */
        if(timeDiff <= 0){
            return;
        }

        /* otherwise nanosleep() */
        waitTime.tv_sec = timeDiff / _oneBillion;
        waitTime.tv_nsec = timeDiff % _oneBillion;
        int retval = nanosleep(&waitTime, NULL);

        /* if nanosleep completed, return */
        if(retval == 0){
            return;
        }
        /* otherwise loop back to start */
    }
}

#endif /* end __linux__*/