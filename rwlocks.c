// #include <semaphore.h>
#include "common_threads.h"
#include "common.h"
#include "rwlocks.h"

// Init counter for acquired and released locks.
void num_locks_init()
{
    num_locks_acqn = 0;
    num_locks_released = 0;

    return;
}

// Getter for locks acquired.
int get_num_locks_acqn()
{
    return num_locks_acqn;
}

// Getter for locks released.
int get_num_locks_released()
{
    return num_locks_released;
}

void rwlock_init() {
    rwlock_t *lock = &mutex;
    
    lock->readers = 0;
    Sem_init(&lock->lock, 1); 
    Sem_init(&lock->writelock, 1); 
}

// Getter for mutex.
rwlock_t get_mutext_lock() { return mutex; }

void rwlock_acquire_readlock() {
    rwlock_t *lock = &mutex;
    
    Sem_wait(&lock->lock);
    lock->readers++;
    if (lock->readers == 1)
	Sem_wait(&lock->writelock);
    Sem_post(&lock->lock);
}

void rwlock_release_readlock() {
    rwlock_t *lock = &mutex;
    
    Sem_wait(&lock->lock);
    lock->readers--;
    if (lock->readers == 0)
	Sem_post(&lock->writelock);
    Sem_post(&lock->lock);
}

void rwlock_acquire_writelock() {
    rwlock_t *lock = &mutex;
    
    Sem_wait(&lock->writelock);
}

void rwlock_release_writelock() {
    rwlock_t *lock = &mutex;
    
    Sem_post(&lock->writelock);
}

