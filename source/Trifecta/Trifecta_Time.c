#include "Trifecta_Time.h"

#include <stdbool.h>

#define oneBillion (1000000000L)

#ifdef __APPLE__

#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

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
    unsigned int secondsToAdd = timeNano / oneBillion;
    uint64_t nanosecondsToAdd = timeNano % oneBillion;
    timePoint.tv_sec += secondsToAdd;
    timePoint.tv_nsec += nanosecondsToAdd;

    /* carry if needed*/
    if(timePoint.tv_nsec >= oneBillion){
        ++(timePoint.tv_sec);
        timePoint.tv_nsec -= oneBillion;
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

    int64_t diff = (secDiff * oneBillion) + nanoDiff;
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
        waitTime.tv_sec = timeDiff / oneBillion;
        waitTime.tv_nsec = timeDiff % oneBillion;
        int retval = nanosleep(&waitTime, NULL);

        /* if nanosleep completed, return */
        if(retval == 0){
            return;
        }
        /* otherwise loop back to start */
    }
}

#endif /* end __APPLE__ */