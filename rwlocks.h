#ifndef _RWLOCKS_H
#define _RWLOCKS_H

#include <semaphore.h>

// Hold lock read/write lock info.
typedef struct _rwlock_t
{
    sem_t writelock;
    sem_t lock;
    int readers;
} rwlock_t;

// Mutex lock.
extern rwlock_t mutex;

extern int num_locks_acqn;
extern int num_locks_released;

extern void num_locks_init();
extern void increment_num_locks_acqn();
extern void increment_num_locks_released();

extern int get_num_locks_acqn();
extern int get_num_locks_released();

extern void rwlock_init();

extern void rwlock_acquire_readlock();
extern void rwlock_release_readlock();

extern void rwlock_acquire_writelock();
extern void rwlock_release_writelock();

#endif