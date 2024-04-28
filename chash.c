#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "file_io.h"
#include "hashdb.h"
#include "rwlocks.h"

void create_threads(pthread_t *threads, int num_threads, op_args *operations, FILE *commands_file)
{
    op_args *args;

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
            pthread_create(&threads[i], NULL, create_record_list, &operations[i]);
        }
    }
}

void join_threads(pthread_t *threads, op_args *operations, int num_threads) {
    for (int i = 0; i < num_threads; i++)
    {
        void *result = NULL;
        
        int res = pthread_join(threads[i], &result);

        if (strcmp(operations[i].op, "search") == 0)
        {
            if (result == NULL)
            {
                write_no_record_found();
            }
            else
            {
                hashRecord *found_record = (hashRecord *)result;

                write_record(found_record->hash, found_record->name, found_record->salary);
            }
        }
    }
}

void chash(void)
{
    // Open file.
    FILE *commands_file = open_commands_file("commands.txt");

    // Read number of threads + 1 for final print.
    int num_threads = read_num_threads(commands_file) + 1;

    pthread_t *threads = (pthread_t *)calloc(num_threads, sizeof(pthread_t));
    op_args *operations = (op_args *)calloc(num_threads, sizeof(op_args));

   // TODO: Add checks for DMA above.

    hashRecord *hash_record_head = NULL;

    // Read operations from commands file.
    for (int i = 0; i < num_threads; i++)
    {
        read_op(commands_file, &operations[i]);
        operations[i].hash_record_head = &hash_record_head;
    }

    // Close commands file.
    close_commands_file(commands_file);

    // Initialize thread locks.
    rwlock_init();
    num_locks_init();

    // Open output file.
    open_output_file("output.txt");

    create_threads(threads, num_threads, operations, commands_file);
    join_threads(threads, operations, num_threads);

    // Close output file.
    close_output_file();

    free(threads);

    free_hash_record(hash_record_head);
    free(operations);

}

int main()
{
    chash();
    return 0;
}
