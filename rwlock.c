/* Licensed under Apache License v2.0 - see LICENSE file for details */

#include <errno.h>
#include "rwlock.h"

void init_rwlock(struct rwlock *rw, int readers)
{
    sem_init(&rw->rlock, 1, 0);
    sem_init(&rw->wlock, 1, 1);
    rw->readers = readers;
}

void destroy_rwlock(struct rwlock *rw)
{
    wlock(rw);

    sem_destroy(&rw->rlock);
    sem_destroy(&rw->wlock);
}

void rlock(struct rwlock *rw)
{
    int readers;
    for (;;)
    {
        sem_wait(&rw->wlock);
        if (sem_getvalue(&rw->rlock, &readers) != -1)
        {
            if (readers < rw->readers)
            {
                sem_post(&rw->rlock);
                sem_post(&rw->wlock);
                break;
            }
        }
        sem_post(&rw->wlock);

        usleep(1);
    }
}

void wlock(struct rwlock *rw)
{
    int readers = 0;

    sem_wait(&rw->wlock);

    for (;;)
    {
        if (sem_getvalue(&rw->rlock, &readers) != -1)
        {
            if (readers == 0)
            {
                break;
            }
        }

        usleep(1);
    }
}

void runlock(struct rwlock *rw)
{
    for (;;)
    {
        if (sem_trywait(&rw->rlock) == -1)
        {
            if (errno != EAGAIN)
            {
                continue;
            }
        }
        break;
    }
}

void wunlock(struct rwlock *rw)
{
    sem_post(&rw->wlock);
}