#ifndef _RWLOCKS_H
#define _RWLOCKS_H

#include <semaphore.h>

typedef struct _rwlock_t {
    sem_t writelock;
    sem_t lock;
    int readers;
} rwlock_t;

rwlock_t mutex;

int num_locks_acqn;
int num_locks_released;

// Init counter for acquired and released locks.
extern void num_locks_init();

extern void increment_num_locks_acqn();

extern void increment_num_locks_released();

// Getter for locks acquired.
extern int get_num_locks_acqn();

// Getter for locks released.
extern int get_num_locks_released();

extern void rwlock_init();

extern rwlock_t get_mutext_lock();

extern void rwlock_acquire_readlock();

extern void rwlock_release_readlock();

extern void rwlock_acquire_writelock();

extern void rwlock_release_writelock();

#endif