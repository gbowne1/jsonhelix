#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

// --- String helpers ---

char* str_trim(char *str) {
    if (!str) return NULL;
    char *end;

    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';

    return str;
}

char* str_dup(const char *src) {
    if (!src) return NULL;
    char *copy = malloc(strlen(src) + 1);
    if (copy) strcpy(copy, src);
    return copy;
}

int str_eq(const char *a, const char *b) {
    if (!a || !b) return 0;
    return strcmp(a, b) == 0;
}

// --- File I/O ---

char* read_file(const char *path) {
    FILE *fp = fopen(path, "rb");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    rewind(fp);

    char *buf = malloc(len + 1);
    if (!buf) {
        fclose(fp);
        return NULL;
    }

    fread(buf, 1, len, fp);
    buf[len] = '\0';
    fclose(fp);
    return buf;
}

int write_file(const char *path, const char *data) {
    FILE *fp = fopen(path, "w");
    if (!fp) return 0;
    fputs(data, fp);
    fclose(fp);
    return 1;
}

// --- Logging ---

static void log_printf(const char *prefix, const char *msg) {
    fprintf(stderr, "[%s] %s\n", prefix, msg);
}

void log_info(const char *msg)  { log_printf("INFO",  msg); }
void log_error(const char *msg) { log_printf("ERROR", msg); }
void log_debug(const char *msg) { log_printf("DEBUG", msg); }
