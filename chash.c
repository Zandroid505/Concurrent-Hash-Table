#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "file_io.h"
#include "hashdb.h"
#include "rwlocks.h"

/*
 * Creates the threads for each operation on the hash record.
 * Arguments:
 *     - Array holding threads for each operation.
 *     - Number of threads.
 *     - Operations performed for each thread.
 *
 * Returns void.
 */
void create_threads(pthread_t *threads, op_args *operations, int num_threads)
{
    for (int i = 0; i < num_threads; i++)
    {
        if (strcmp(operations[i].op, "insert") == 0)
        {
            pthread_create(&threads[i], NULL, insert, &operations[i]);
        }
        else if (strcmp(operations[i].op, "delete") == 0)
        {
            pthread_create(&threads[i], NULL, delete, &operations[i]);
        }
        else if (strcmp(operations[i].op, "search") == 0)
        {
            pthread_create(&threads[i], NULL, search, &operations[i]);
        }
        else if (strcmp(operations[i].op, "print") == 0 || strcmp(operations[i].op, "final") == 0)
        {
            pthread_create(&threads[i], NULL, print_hash_record, &operations[i]);
        }
    }

    return;
}

/*
 * Join threads and get thread returns if there are any.
 * Arguments:
 *     - Array holding threads for each operation.
 *     - Number of threads.
 *     - Operations performed for each thread.
 *
 * Returns void.
 */
void join_threads(pthread_t *threads, op_args *operations, int num_threads)
{
    // Hold found records for printing to output file.
    hashRecord *found_records = (hashRecord *)calloc(num_threads, sizeof(hashRecord));
    int found_record_index = 0;

    for (int i = 0; i < num_threads; i++)
    {
        void *result = NULL;

        pthread_join(threads[i], &result);

        if (strcmp(operations[i].op, "search") == 0)
        {
            if (result == NULL)
            {
                write_no_record_found();
            }
            else
            {
                hashRecord *found_record = (hashRecord *)result;

                found_records[found_record_index] = *(found_record);
                write_record(found_record->hash, found_record->name, found_record->salary);
            }
        }
    }

    // Iterate through found records and write them to file.
    
    free(found_records);

    return;
}

/*
 * Perform commands specified on commands.txt using threaded hash record.
 * Arguments: None
 *
 * Returns void.
 */
void chash(void)
{
    // Open file.
    open_input_file("commands.txt");
    FILE *input_file = get_input_file();

    // Read number of threads (+ 1 for final print).
    int num_threads = read_num_threads(input_file) + 1;

    // Allocate threads.
    pthread_t *threads = (pthread_t *)calloc(num_threads, sizeof(pthread_t));
    if (threads == NULL)
    {
        fprintf(stderr, "Error: Could not allocate memory for threads.\n");
        exit(1);
    }

    // Allocated operations.
    op_args *operations = (op_args *)calloc(num_threads, sizeof(op_args));
    if (operations == NULL)
    {
        fprintf(stderr, "Error: Could not allocate memory for hash record operations.\n");
        exit(1);
    }

    hashRecord *hash_record_head = NULL;

    // Read operations from commands file.
    for (int i = 0; i < num_threads; i++)
    {
        read_op(input_file, &operations[i]);
        operations[i].hash_record_head = &hash_record_head;
    }

    // Close commands file.
    close_input_file();

    // Initialize thread locks.
    rwlock_init();
    num_locks_init();

    // Open output file.
    open_output_file("output.txt");

    create_threads(threads, operations, num_threads);
    join_threads(threads, operations, num_threads);

    // Close output file.
    close_output_file();

    free(threads);
    free(operations);

    free_hash_record(hash_record_head);

    return;
}

int main()
{
    chash();
    return 0;
}
