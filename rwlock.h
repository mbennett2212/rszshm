/* Licensed under Apache License v2.0 - see LICENSE file for details */
#ifndef _RWLOCK_H
#define _RWLOCK_H

#include <semaphore.h>

struct rwlock {
    sem_t rlock;
    sem_t wlock;
    int readers;
};

void init_rwlock(struct rwlock *rw, int readers);
void destroy_rwlock(struct rwlock *rw);
void rlock(struct rwlock *rw);
void runlock(struct rwlock *rw);
void wlock(struct rwlock *rw);
void wunlock(struct rwlock *rw);

#endif /* _RWLOCK_H */