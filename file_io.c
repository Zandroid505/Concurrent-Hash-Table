#include <stdlib.h>
#include "file_io.h"
#include "hashdb.h"

// Output file.
FILE *output_file;

/*
* Open commands file.
* Arguments:
*     - File name.
* Returns opened file pointer.
*/
FILE *open_commands_file(char *file_name)
{
    FILE *commands_file = fopen(file_name, "r");
	
    if (commands_file == NULL)
	{
		fprintf(stderr, "Error opening file %s.\n", file_name);
		exit(1);
	}

    return commands_file;
}

/*
* Read number of threads from the commands file.
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
* Returns struct containing operation details.
*/
int read_op(FILE *commands_file, op_args *operation)
{
	if (fscanf(commands_file, "%[^,],%[^,],%u\n", operation->op, operation->name, &(operation->salary)) != EOF)
		return 0;
	else
	{
		return -1;
	}
}

/*
* Closes commands file.
* Arguments:
*     - File pointer.
* Returns void.
*/
void close_commands_file(FILE* commands_file)
{
    if (fclose(commands_file) != 0)
    {
        fprintf(stderr, "Error closing commands file.\n");
		exit(1);
    }
}

/*
* Open output file.
* Arguments:
*     - File name.
* Returns opened file pointer.
*/
void open_output_file(char *file_name)
{
    output_file = fopen(file_name, "w");
	
    if (output_file == NULL)
	{
		fprintf(stderr, "Error opening file %s.\n", file_name);
		exit(1);
	}
}

/*
* Get output file.
* Arguments:
* 
* Returns file pointer.
*/
FILE *get_output_file()
{
    return output_file;
}


/*
* Write the insert operation to the output file.
* Arguments:
*     - Operation type.
*     - Hash value associated with hash record.
*     - Name of person.
*     - Salary of person.
* Returns void.
*/
void write_insert_op(char op[15], uint32_t hash, char name[50], uint32_t salary)
{
    fprintf(output_file, "%s,%u,%s,%u\n", op, hash, name, salary);
}

/*
* Write the insert operation to the output file.
* Arguments:
*     - Operation type.
*     - Name of person.
* Returns void.
*/
void write_delete_op(char op[15], char name[50])
{
    fprintf(output_file, "%s %s\n", op, name);
}

/*
* Write the insert operation to the output file.
* Arguments:
*     - Operation type.
*     - Name of person.
* Returns void.
*/
void write_search_op(char op[15], char name[50])
{
    fprintf(output_file, "%s %s\n", op, name);
}

void write_record(uint32_t hash, char name[50], uint32_t salary)
{
    fprintf(output_file, "%u,%s,%u\n", hash, name, salary);
}

void write_no_record_found()
{
    fprintf(output_file, "No Record Found\n");
}


/*
* Write "WRITE LOCK ACQUIRED" to output file.
* Arguments: N/A
* Returns void.
*/
void write_write_lock_acquired()
{
    fprintf(output_file, "WRITE LOCK ACQUIRED\n");
}

/*
* Write "WRITE LOCK RELEASED" to output file.
* Arguments: N/A
* Returns void.
*/
void write_write_lock_released()
{
    fprintf(output_file, "WRITE LOCK RELEASED\n");
}

/*
* Write "READ LOCK ACQUIRED" to output file.
* Arguments: N/A
* Returns void.
*/
void write_read_lock_acquired()
{
    fprintf(output_file, "READ LOCK ACQUIRED\n");
}

/*
* Write "READ LOCK RELEASED" to output file.
* Arguments: N/A
* Returns void.
*/
void write_read_lock_released()
{
    fprintf(output_file, "READ LOCK RELEASED\n");
}

/*
* Write entire record to file.
* Arguments:
*     - Formatted string of entire hash record.
* Returns void.
*/
void write_record_list(char *record_list)
{
    fprintf(output_file, "%s", record_list);
}

void write_final_print(int locks_acquired, int locks_released, char *record_list)
{
    fprintf(output_file, "Number of lock acquisitions: %d\n", locks_acquired);
    fprintf(output_file, "Number of lock releases: %d\n", locks_released);
    fprintf(output_file, "Final Table:\n");
    fprintf(output_file, "%s", record_list);
}


/*
* Closes output file.
* Arguments: N/A
* Returns void.
*/
void close_output_file()
{
    if (fclose(output_file) != 0)
    {
        fprintf(stderr, "Error closing output file.\n");
		exit(1);
    }
}

