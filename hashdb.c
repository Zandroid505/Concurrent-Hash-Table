#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "hashdb.h"
#include "file_io.h"
#include "rwlocks.h"

// Jenkins Hash function declaration.
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

// Insertion function declaration.
void *insert(void *arg)
{
	op_args *args = (op_args *)arg;
	uint32_t hash = jenkins_hash(args->name);

	// Write the operation to the file.
	write_insert_op(args->op, hash, args->name, args->salary);

	// write_write_lock_acquired();

	// rwlock_acquire_writelock();

	// Create new record & check for memory alloc errors
	hashRecord *new_record = malloc(sizeof(hashRecord));
	if (new_record == NULL)
	{
		printf("Error: Could not allocate memory for new record.\n");
		return NULL;
	}

	// Initialize new record
	new_record->hash = hash;
	strncpy(new_record->name, args->name, 50);
	new_record->salary = args->salary;
	new_record->next = NULL;

	// Find correct position and insert
	hashRecord **head = &(args->hash_record_head);

	// If hashRecord is empty, make it head.
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

	// rwlock_release_writelock();

	// write_write_lock_released();

	// pthread_exit(NULL)
}

// Deletion function declaration.
void delete(void *arg)
{
	op_args *args = (op_args *)arg;
	uint32_t hash = jenkins_hash(args->name);
	char name[50];

	// Write delete operation to file.
	write_delete_op(args->op, args->name);

	// write_write_lock_acquired();

	// rwlock_acquire_writelock();

	// Hold the head and init a prev hashrecord holder.
	hashRecord *temp = args->hash_record_head;
	hashRecord *prev = NULL;
	
	// If head is the record to be deleted.
	if (temp != NULL && temp->hash == hash)
	{
		args->hash_record_head = temp->next;
		free(temp);
		return;
	}

	// Search hashRecord for record.
	while (temp != NULL && temp->hash != hash)
	{
		prev = temp;
		temp = temp->next;
	}

	// If found, then delete
	if (temp != NULL)
	{
		strncpy(name, temp->name, 50);

		// Unlink record from hashrecord.
		prev->next = temp->next;

		free(temp);
	}

	// rwlock_release_writelock();

	// write_write_lock_released();

	// pthread_exit(NULL);
}

// Search function declaration.
void *search(void *arg)
{
	op_args *args = (op_args *)arg;
	uint32_t hash = jenkins_hash(args->name);

	// Write the operation to the file.
	write_search_op(args->op, args->name);

	// write_read_lock_acquired();

	// rwlock_acquire_readlock();

	// Search list for record
	hashRecord *current = args->hash_record_head;
	while (current != NULL)
	{
		if (current->hash == hash)
		{
			return current;
		}
		
		current = current->next;
	}

	// rwlock_release_readlock();

	// write_read_lock_released();

	// pthread_exit(found_record);


	return NULL;
}

void *create_record_list(void *arg)
{
	op_args *args = (op_args *)arg;

	// write_read_lock_acquired();

	// rwlock_acquire_readlock();

	// Initialize string to hold list contents
	char *list_contents = malloc(1);
	list_contents[0] = '\0';

	hashRecord *current = args->hash_record_head;
	while (current != NULL)
	{
		// Create string for record
		char record[100];

		// Uses snprintf to prevent buffer overflow
		// snprintf(record, sizeof(record), "Hash: %u, Name: %s, Salary: %u\n", current->hash, current->name, current->salary);

		// This version is closer to expected output
		snprintf(record, sizeof(record), "%u,%s,%u\n", current->hash, current->name, current->salary);

		// Reallocate string to hold new record
		list_contents = realloc(list_contents, strlen(list_contents) + strlen(record) + 1);
		if (list_contents == NULL)
		{
			printf("Error: Could not allocate memory for list_contents\n");
			return NULL;
		}

		// Append to list_contents
		strcat(list_contents, record);

		// Go to next record
		current = current->next;
	}

	// rwlock_release_readlock();

	// write_read_lock_released();

	// pthread_exit(list_contents);

	return list_contents;
}

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

	// free(hash_record_head)
}