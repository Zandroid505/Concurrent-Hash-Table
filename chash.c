#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "file_io.h"
#include "hashdb.h"
#include "rwlocks.h"

void create_threads(pthread_t *threads, op_args *operations, hashRecord *hash_record_head, FILE *commands_file)
{
    int i = 0;
    op_args *args;

    while ((read_op(commands_file, &operations[i])) == 0)
    {
        // Assign head to new operation from previous operation.
        if (i > 0 && operations[i].hash_record_head == NULL)
            operations[i].hash_record_head = operations[i - 1].hash_record_head;

        if (strcmp(operations[i].op, "insert") == 0)
        {
            insert(&operations[i]);
            // pthread_create(&threads[i], NULL, insert, args)
        }
        else if (strcmp(operations[i].op, "delete") == 0)
        {

            delete(&operations[i]);
            // pthread_create(&threads[i], NULL, delete, args);
        }
        else if (strcmp(operations[i].op, "search") == 0)
        {

            void *found_record_temp = search(&operations[i]);

            hashRecord *found_record = (hashRecord *)found_record_temp;

            if (found_record != NULL)
                write_record(found_record->hash, found_record->name, found_record->salary);
            else
                write_no_record_found();

            // pthread_create(&threads[i], NULL, search, args);
        }
        else if (strcmp(operations[i].op, "print") == 0)
        {
            void *record_list_tmp = create_record_list(&operations[i]);

            char *record_list = (char *)record_list_tmp;
            
            write_record_list(record_list);
            free(record_list);
            // pthread_create(&threads[i], NULL, create_record_list, args);
        }
        i++;
    }
}

// void join_threads(pthread_t *threads, char *op_type, int num_threads) {
//     for (int i = 0; i < num_threads; i++)
//     {
//         void *result = NULL;

//         if (op_type[i] == 's')
//         {
//             pthread_join(threads[i], &result);

//             if (!result)
//             {
//                 write_no_record_found();
//             }
//             else
//             {
//                 write_record((hashRecord *)result);
//             }
//         }
//         else if (op_type[i] == 'p')
//         {
//             pthread_join(threads[i], &result);
//             write_record_list(result);
//         }
//     }
// }

void chash(void)
{
    // Open file.
    FILE *commands_file = open_commands_file("commands.txt");

    // Read number of threads.
    int num_threads = read_num_threads(commands_file);

    pthread_t *threads = (pthread_t *)calloc(num_threads, sizeof(pthread_t));
    op_args *operations = (op_args *)calloc(num_threads, sizeof(op_args));

    // Init head pointers for each operation struct.
    for (int i = 0; i < num_threads; i++)
    {
        operations[i].hash_record_head = NULL;
    }

    // TODO: Add checks for DMA above.

    hashRecord *hash_record_head = NULL;

    rwlock_init();
    num_locks_init();

    // Open output file.
    open_output_file("output.txt");

    create_threads(threads, operations, hash_record_head, commands_file);
    // join_threads(threads, op_type, num_threads);

    void *record_list_tmp = create_record_list(&operations[num_threads - 1]);

    char *record_list = (char *)record_list_tmp;

    write_final_print(0, 0, record_list);

    // Free allocated final print message.
    free(record_list);

    // Close files.
    close_commands_file(commands_file);
    close_output_file();

    free(threads);

    // Free hash Record.
    free_hash_record(operations[num_threads - 1].hash_record_head);

    free(operations);

}

int main()
{
    chash();
    return 0;
}
