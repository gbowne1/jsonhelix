#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

// String helpers
char* str_trim(char *str);
char* str_dup(const char *src);
int str_eq(const char *a, const char *b);

// File I/O
char* read_file(const char *path);
int write_file(const char *path, const char *data);

// Logging
void log_info(const char *msg);
void log_error(const char *msg);
void log_debug(const char *msg);

#endif
