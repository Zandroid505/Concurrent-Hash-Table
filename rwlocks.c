#include "common_threads.h"
#include "common.h"
#include "rwlocks.h"

rwlock_t mutex;

int num_locks_acqn;
int num_locks_released;

/*
 * Initialize the number of locks acquired and released to 0.
 * Arguments: None.
 *
 * Returns void.
 */
void num_locks_init()
{
    num_locks_acqn = 0;
    num_locks_released = 0;

    return;
}

/*
 * Increment the number of locks acquired by 1.
 * Arguments: None.
 *
 * Returns void.
 */
void increment_num_locks_acqn()
{
    num_locks_acqn++;

    return;
}

/*
 * Increment the number of locks released by 1.
 * Arguments: None.
 *
 * Returns void.
 */
void increment_num_locks_released()
{
    num_locks_released++;

    return;
}

/*
 * Get the number of locks acquired.
 * Arguments: None.
 *
 * Returns number of locks acquired.
 */
int get_num_locks_acqn()
{
    return num_locks_acqn;
}

/*
 * Get the number of locks released.
 * Arguments: None.
 *
 * Returns number of locks released.
 */
int get_num_locks_released()
{
    return num_locks_released;
}

/*
 * Initialize the mutex lock.
 * Arguments: None.
 *
 * Returns void.
 */
void rwlock_init()
{
    rwlock_t *lock = &mutex;

    lock->readers = 0;
    Sem_init(&lock->lock, 1);
    Sem_init(&lock->writelock, 1);

    return;
}

/*
 * Acquire the read lock.
 * Arguments: None.
 *
 * Returns void.
 */
void rwlock_acquire_readlock()
{
    rwlock_t *lock = &mutex;

    Sem_wait(&lock->lock);
    lock->readers++;
    if (lock->readers == 1)
        Sem_wait(&lock->writelock);
    Sem_post(&lock->lock);

    return;
}

/*
 * Release the read lock.
 * Arguments: None.
 *
 * Returns void.
 */
void rwlock_release_readlock()
{
    rwlock_t *lock = &mutex;

    Sem_wait(&lock->lock);
    lock->readers--;
    if (lock->readers == 0)
        Sem_post(&lock->writelock);
    Sem_post(&lock->lock);

    return;
}

/*
 * Acquire the write lock.
 * Arguments: None.
 *
 * Returns void.
 */
void rwlock_acquire_writelock()
{
    rwlock_t *lock = &mutex;

    Sem_wait(&lock->writelock);

    return;
}

/*
 * Release the write lock.
 * Arguments: None.
 *
 * Returns void.
 */
void rwlock_release_writelock()
{
    rwlock_t *lock = &mutex;

    Sem_post(&lock->writelock);

    return;
}
