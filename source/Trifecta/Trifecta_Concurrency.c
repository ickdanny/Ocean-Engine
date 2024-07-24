#include "Trifecta_Concurrency.h"

#include "PGUtil.h"

#ifdef __APPLE__

#include <sys/errno.h>
#include <signal.h>

/*
 * Initializes the current thread; should be called by
 * runnable functions
 */
void initThread(){
    /* set the pthread thread as cancellable */
    pthread_setcancelstate(
        PTHREAD_CANCEL_ENABLE,
        NULL
    );
    pthread_setcanceltype(
        PTHREAD_CANCEL_ASYNCHRONOUS,
        NULL
    );
}

/* 
 * Creates a new thread and returns its ID if
 * successful
 */
CreateReturn threadCreate(
    RunnableFuncPtr func,
    void* arg
){
    CreateReturn toRet;
    int result = pthread_create(
        &(toRet.thread),
        NULL,
        func,
        arg
    );
    toRet.success = (result == 0);

    return toRet;
}

/* Kills the specified thread */
void threadKill(Thread toKill){
   pthread_cancel(toKill);
}

/* Attempts to join the specified thread */
JoinReturnCode threadJoin(Thread thread){
    int result = pthread_join(thread, NULL);

    /* if result is not 0, an error occurred */
    if(result != 0){
        switch(errno){
            case EINVAL:
                return joinNotJoinable;
            case ESRCH:
                return joinNoSuchThread;
            case EDEADLK:
                return joinDeadlock;
            default:
                return joinUnknownError;
        }
    }
    return joinSuccess;
}

#endif /* end __APPLE__ */