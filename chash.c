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
        // Assign head to new operation from previous operation.
        // if (i > 0 && operations[i].hash_record_head == NULL)
        //     operations[i].hash_record_head = operations[i - 1].hash_record_head;

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
        // else if (strcmp(operations[i].op, "print") == 0)
        // {
        //     void *record_list_tmp = create_record_list(&operations[i]);

        //     char *record_list = (char *)record_list_tmp;
            
        //     write_record_list(record_list);
        //     free(record_list);
        //     // pthread_create(&threads[i], NULL, create_record_list, args);
        // }
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
//         else if (op_type[i] == 'p')
//         {
//             pthread_join(threads[i], &result);
//             write_record_list(result);
//         }
    }
}

void chash(void)
{
    // Open file.
    FILE *commands_file = open_commands_file("commands1.txt");

    // Read number of threads.
    int num_threads = read_num_threads(commands_file);

    pthread_t *threads = (pthread_t *)calloc(num_threads, sizeof(pthread_t));
    op_args *operations = (op_args *)calloc(num_threads, sizeof(op_args));

   // TODO: Add checks for DMA above.

    hashRecord *hash_record_head = NULL;

    // Read operations from commands file.
    for (int i = 0; i < num_threads && (read_op(commands_file, &operations[i])) == 0; i++)
        operations[i].hash_record_head = &hash_record_head;

    // Close commands file.
    close_commands_file(commands_file);

    // Initialize thread locks.
    rwlock_init();
    num_locks_init();

    // Open output file.
    open_output_file("output.txt");

    create_threads(threads, num_threads, operations, commands_file);
    join_threads(threads, operations, num_threads);

    // void *record_list_tmp = create_record_list(&operations[num_threads - 1]);

    // char *record_list = (char *)record_list_tmp;

    // write_final_print(0, 0, record_list);

    // Free allocated final print message.
    // free(record_list);

    // Close output file.
    close_output_file();

    free(threads);

    // hashRecord *current = *(operations[0].hash_record_head);
    // while (current != NULL)
    // {
    //     printf("%s\n", current->name);
    //     current = current->next;
    // }

    free_hash_record(hash_record_head);
    free(operations);

}

int main()
{
    chash();
    return 0;
}
