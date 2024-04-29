#include <stdlib.h>
#include <string.h>

#include "file_io.h"
#include "hashdb.h"

FILE *input_file;
FILE *output_file;

/*
 * Open input file.
 * Arguments:
 *     - File name.
 * Returns void.
 */
void open_input_file(char *file_name)
{
    input_file = fopen(file_name, "r+");

    if (input_file == NULL)
    {
        fprintf(stderr, "Error opening file %s.\n", file_name);
        exit(1);
    }

    return;
}

/*
 * Get input file.
 * Arguments: None
 *
 * Returns input file pointer.
 */
FILE *get_input_file() { return input_file; }

/*
 * Read number of threads from the input file.
 * Arguments:
 *     - File pointer containing commands.
 * Returns number of threads.
 */
int read_num_threads(FILE *commands_file)
{
    int num_threads;
    fscanf(commands_file, "threads,%d,0\n", &num_threads);

    return num_threads;
}

/*
 * Read operation from commands file.
 * Arguments:
 *     - File pointer containing commands.
 *     - Operation struct to hold operation info.
 * Returns 0 if there are still operations and -1 if it reaches the end of the file.
 */
int read_op(FILE *commands_file, op_args *operation)
{
    char *line = NULL;
    char *token;
    size_t len = 0;

    // Read line from file.
    if (getline(&line, &len, commands_file) != -1)
    {

        token = strtok(line, ",");

        // Get op.
        strncpy(operation->op, token, 15);

        token = strtok(NULL, ",");

        // Get name.
        strncpy(operation->name, token, 50);

        token = strtok(NULL, ",");

        // Get salary.
        sscanf(token, "%u", &operation->salary);

        // Free allocated line.
        free(line);

        return 0;
    }
    // End of file, set op to final print.
    else
    {
        // Set op type to final print.
        strncpy(operation->op, "final", 15);

        // Init name to null character, won't be used.
        strncpy(operation->name, "\0", 50);

        // Init salary, won't be used.
        operation->salary = 0;

        free(line);
        return -1;
    }
}

/*
 * Closes input file.
 * Arguments: None
 *
 * Returns void.
 */
void close_input_file()
{
    if (fclose(input_file) != 0)
    {
        fprintf(stderr, "Error closing commands file.\n");
        exit(1);
    }

    return;
}

/*
 * Open output file.
 * Arguments:
 *     - File name.
 *
 * Returns void.
 */
void open_output_file(char *file_name)
{
    output_file = fopen(file_name, "w+");

    if (output_file == NULL)
    {
        fprintf(stderr, "Error opening file %s.\n", file_name);
        exit(1);
    }

    return;
}

/*
 * Get output file.
 * Arguments: None
 *
 * Returns output file pointer.
 */
FILE *get_output_file() { return output_file; }

/*
 * Write the insert operation to the output file.
 * Arguments:
 *     - Operation type.
 *     - Hash of the record.
 *     - Name of person.
 *     - Salary of person.
 *
 * Returns void.
 */
void write_insert_op(char op[15], uint32_t hash, char name[50], uint32_t salary)
{
    fprintf(output_file, "INSERT,%u,%s,%u\n", hash, name, salary);

    return;
}

/*
 * Write the delete operation to the output file.
 * Arguments:
 *     - Operation type.
 *     - Name of person.
 *
 * Returns void.
 */
void write_delete_op(char op[15], char name[50])
{
    fprintf(output_file, "DELETE,%s\n", name);

    return;
}

/*
 * Write the search operation to the output file.
 * Arguments:
 *     - Operation type.
 *     - Name of person.
 *
 * Returns void.
 */
void write_search_op(char op[15], char name[50])
{
    fprintf(output_file, "SEARCH,%s\n", name);

    return;
}

/*
 * Write single record to the output file.
 * Arguments:
 *     - Hash of the record.
 *     - Name of person associated with the record.
 *     - Salary of the person assocaited with the record.
 *
 * Returns void.
 */
void write_record(uint32_t hash, char name[50], uint32_t salary)
{
    fprintf(output_file, "%u,%s,%u\n", hash, name, salary);

    return;
}

/*
 * Write "No Record Found" to the output file.
 * Arguments: None.
 *
 * Returns void.
 */
void write_no_record_found()
{
    fprintf(output_file, "No Record Found\n");

    return;
}

/*
 * Write "WRITE LOCK ACQUIRED" to output file.
 * Arguments: None.
 *
 * Returns void.
 */
void write_write_lock_acquired()
{
    fprintf(output_file, "WRITE LOCK ACQUIRED\n");

    return;
}

/*
 * Write "WRITE LOCK RELEASED" to output file.
 * Arguments: None.
 *
 * Returns void.
 */
void write_write_lock_released()
{
    fprintf(output_file, "WRITE LOCK RELEASED\n");

    return;
}

/*
 * Write "READ LOCK ACQUIRED" to output file.
 * Arguments: None.
 *
 * Returns void.
 */
void write_read_lock_acquired()
{
    fprintf(output_file, "READ LOCK ACQUIRED\n");

    return;
}

/*
 * Write "READ LOCK RELEASED" to output file.
 * Arguments: None.
 *
 * Returns void.
 */
void write_read_lock_released()
{
    fprintf(output_file, "READ LOCK RELEASED\n");

    return;
}

/*
 * Write a final print of the hash record to the output file with the
 * number of locks acquired and released.
 * Arguments:
 *     - Number of locks acquired.
 *     - number of locks released.
 *
 * Returns void.
 */
void write_final_print_header(int locks_acquired, int locks_released)
{
    fprintf(output_file, "Number of lock acquisitions: %d\n", locks_acquired);
    fprintf(output_file, "Number of lock releases: %d\n", locks_released);
    fprintf(output_file, "Final Table:\n");

    return;
}

/*
 * Closes output file.
 * Arguments: None.
 *
 * Returns void.
 */
void close_output_file()
{
    if (fclose(output_file) != 0)
    {
        fprintf(stderr, "Error closing output file.\n");
        exit(1);
    }

    return;
}
