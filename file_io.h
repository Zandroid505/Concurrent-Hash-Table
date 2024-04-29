#ifndef _FILE_IO_H
#define _FILE_IO_H

#include <stdio.h>
#include "hashdb.h"

extern FILE *input_file;
extern FILE *output_file;

extern void open_input_file(char *file_name);
extern FILE *get_input_file();

extern int read_num_threads(FILE *commands_file);
extern int read_op(FILE *commands_file, op_args *operation);

extern void close_input_file();

extern void open_output_file(char *file_name);
extern FILE *get_output_file();

// Write operations.
extern void write_insert_op(char op[15], uint32_t hash, char name[50], uint32_t salary);
extern void write_delete_op(char op[15], char name[50]);
extern void write_search_op(char op[15], char name[50]);

extern void write_record(uint32_t hash, char name[50], uint32_t salary);
extern void write_no_record_found();

// Write lock messages.
extern void write_write_lock_acquired();
extern void write_write_lock_released();

// Read lock messages.
extern void write_read_lock_acquired();
extern void write_read_lock_released();

extern void write_record_list(char *record_list);
extern void write_final_print_header(int locks_acquired, int locks_released);

extern void close_output_file();

#endif
