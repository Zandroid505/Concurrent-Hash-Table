#ifndef _HASHDB_H
#define _HASHDB_H
#include <stdint.h>

// Hash structure.
// Basically a linked list sorted by hash value.
typedef struct hash_struct
{
	uint32_t hash;
	char name[50];
	uint32_t salary;
	struct hash_struct *next;
} hashRecord;

// Hold arguments to pass to hash record operation functions.
typedef struct hash_record_op_arg
{
	hashRecord **hash_record_head;
	char op[15];
	char name[50];
	uint32_t salary;
} op_args;

extern uint32_t jenkins_hash(char *key);

extern void *insert(void *args);
extern void *delete(void *args);
extern void *search(void *args);

extern void *print_hash_record(void *args);
extern void free_hash_record(hashRecord *hash_record_head);

#endif
