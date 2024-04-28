#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "hashdb.h"
#include "file_io.h"
#include "rwlocks.h"

/*
 * Calculates Jenkin's one_at_a_time hash based on an inputted name.
 * Arguments:
 *     - Name of a person.
 *
 * Returns calculated hash value.
 */
uint32_t jenkins_hash(char name[50])
{
	size_t length = strnlen(name, 50);
	uint32_t hash = 0;
	size_t i;

	for (i = 0; i < length; ++i)
	{
		hash += name[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

/*
 * Inserts a new record into the hash record.
 * Arguments:
 *     - arg
 *         - Pointer to the head of the hash record.
 *         - Operation type of record ("insert").
 *         - Name of the new record.
 *         - Salary of the new record.
 *
 * Returns NULL.
 */
void *insert(void *arg)
{
	op_args *args = (op_args *)arg;
	uint32_t hash = jenkins_hash(args->name);

	// Create new record & check for memory alloc errors.
	hashRecord *new_record = malloc(sizeof(hashRecord));
	if (new_record == NULL)
	{
		fprintf(stderr, "Error: Could not allocate memory for new record.\n");
		return NULL;
	}

	rwlock_acquire_writelock();

	increment_num_locks_acqn();
	write_write_lock_acquired();

	write_insert_op(args->op, hash, args->name, args->salary);

	// Initialize new record.
	new_record->hash = hash;
	strncpy(new_record->name, args->name, 50);
	new_record->salary = args->salary;
	new_record->next = NULL;

	// Get head.
	hashRecord **head = args->hash_record_head;

	// If hashRecord is empty or new_record has hash greater than current head, make it head.
	if (*head == NULL || (*head)->hash >= new_record->hash)
	{
		new_record->next = *head;
		*head = new_record;
	}
	else
	{
		hashRecord *current = *head;

		while (current->next != NULL && current->next->hash < hash)
		{
			current = current->next;
		}

		new_record->next = current->next;
		current->next = new_record;
	}

	increment_num_locks_released();
	write_write_lock_released();

	rwlock_release_writelock();

	pthread_exit(NULL);
}

/*
 * Deletes a record from the hash record.
 * Arguments:
 *     - arg
 *         - Pointer to the head of the hash record.
 *         - Operation type of record ("delete").
 *         - Name of the record to delete.
 *         - Salary (NOT USED).
 *
 * Returns NULL.
 */
void *delete(void *arg)
{
	op_args *args = (op_args *)arg;
	uint32_t hash = jenkins_hash(args->name);
	char name[50];

	rwlock_acquire_writelock();

	increment_num_locks_acqn();
	write_write_lock_acquired();

	write_delete_op(args->op, args->name);

	// Hold the head and init a prev hashrecord holder.
	hashRecord **head = args->hash_record_head;
	hashRecord *prev = NULL;

	// If head is the record to be deleted.
	if (*head != NULL && (*head)->hash == hash)
	{
		hashRecord *temp = (*head);
		*head = (*head)->next;
		free(*head);
	}
	else
	{
		hashRecord *current = *head;

		// Search hashRecord for record.
		while (current != NULL && current->hash != hash)
		{
			prev = current;
			current = current->next;
		}

		// If found, then delete.
		if (current != NULL)
		{
			strncpy(name, current->name, 50);

			// Unlink record from hashrecord.
			prev->next = current->next;

			free(current);
		}
	}

	increment_num_locks_released();
	write_write_lock_released();

	rwlock_release_writelock();

	pthread_exit(NULL);
}

/*
 * Searches for a record in the hash record.
 * Arguments:
 *     - arg
 *         - Pointer to the head of the hash record.
 *         - Operation type of record ("search").
 *         - Name of the record to search for.
 *         - Salary (NOT USED).
 *
 * Returns found record or NULL if not found.
 */
void *search(void *arg)
{
	op_args *args = (op_args *)arg;
	uint32_t hash = jenkins_hash(args->name);
	hashRecord *found_record = NULL;

	rwlock_acquire_readlock();

	increment_num_locks_acqn();
	write_read_lock_acquired();

	write_search_op(args->op, args->name);

	// Search list for record
	hashRecord *current = *(args->hash_record_head);
	while (current != NULL)
	{
		if (current->hash == hash)
		{
			found_record = current;
			break;
		}

		current = current->next;
	}

	increment_num_locks_released();
	write_read_lock_released();

	rwlock_release_readlock();

	pthread_exit(found_record);
}

/*
 * Print the entire hash record.
 * Arguments:
 *     - arg
 *         - Pointer to the head of the hash record.
 *         - Operation type of record ("print" or "final").
 *         - Name of the record (NOT USED).
 *         - Salary (NOT USED).
 *
 * Returns NULL.
 */
void *print_hash_record(void *arg)
{
	op_args *args = (op_args *)arg;

	rwlock_acquire_readlock();

	increment_num_locks_acqn();
	write_read_lock_acquired();

	// Print final table stats if final.
	if (strcmp(args->op, "final") == 0)
		// Adding 1 to locks released since this is called before lock is released.
		write_final_print_header(get_num_locks_acqn(), get_num_locks_released() + 1);

	hashRecord *current = *(args->hash_record_head);
	while (current != NULL)
	{
		write_record(current->hash, current->name, current->salary);

		// Go to next record.
		current = current->next;
	}

	write_read_lock_released();
	increment_num_locks_released();

	rwlock_release_readlock();

	pthread_exit(NULL);
}

/*
 * Free each record in the hash record.
 * Arguments:
 *     - Head pointer of the hash record.
 *
 * Returns void.
 */
void free_hash_record(hashRecord *hash_record_head)
{
	hashRecord *current = hash_record_head;
	hashRecord *next;

	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}

	return;
}