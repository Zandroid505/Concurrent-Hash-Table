#ifndef _FILE_IO_H
#define _FILE_IO_H

#include <stdio.h>
#include "hashdb.h"

// #include "hashdb.h"

// Open File.
extern FILE *open_commands_file(char *file_name);

// Read number of threads.
extern int read_num_threads(FILE *commands_file);

// Read operation.
extern int read_op(FILE *commands_file, op_args *operation);

// Close commands file.
extern void close_commands_file(FILE *commands_file);

// Open output file.
extern void open_output_file(char *file_name);

// Get output file.
extern FILE *get_output_file();

// Write operations.
extern void write_insert_op(char op[15], uint32_t hash, char name[50], uint32_t salary);
extern void write_delete_op(char op[15], char name[50]);
extern void write_search_op(char op[15], char name[50]);

// Write record if found.
extern void write_record(uint32_t hash, char name[50], uint32_t salary);

// Write No record found if record not found.
extern void write_no_record_found();

// Write lock messages.
extern void write_write_lock_acquired();
extern void write_write_lock_released();

// Read lock messages.
extern void write_read_lock_acquired();
extern void write_read_lock_released();

// Write entire record list.
extern void write_record_list(char *record_list);

// Write final output
extern void write_final_print(int locks_acquired, int locks_released, char *record_list);

// Close output file.
extern void close_output_file();

// // Read name.
// extern char *read_name();

// // Read salary.
// extern uint32_t read_salary();

// // Write Insert operation.
// extern void write_insert_op(hashRecord *record);

// // Write Delete operation.
// extern void write_delete_op(hashRecord *record);

// // Write Search operation.
// extern void write_search_op(hashRecord *record);

// // Write Read lock acquired.
// extern void write_read_lock_acquired();

// // Write Read lock released.
// extern void write_read_lock_released();

// // Write Write lock acquired.
// extern void write_write_lock_acquired();

// // Write Write lock released.
// extern void write_write_lock_released();

// // Write Single record.
// extern void write_record(hashRecord *record);

// extern void write_no_record_found();

// // Print record.
// extern void write_record_list(hashRecord *record);

// extern void write_final_print();

// extern void close_file();

#endif
