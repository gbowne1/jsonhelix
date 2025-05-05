#ifndef JSON_TOOLS_H
#define JSON_TOOLS_H

#include "json_parser.h"

typedef enum {
    SORT_ASCENDING,
    SORT_DESCENDING
} SortOrder;

// Sorting
void sort_json_object(JsonValue *object, SortOrder order);
void sort_json_array(JsonValue *array, SortOrder order);

// Searching
const JsonValue* find_json_key(const JsonValue *object, const char *key);
int json_key_exists(const JsonValue *object, const char *key);
int json_array_length(const JsonValue *array);

// Conversion (stubs for now)
char* json_to_minified_string(const JsonValue *value);
char* json_to_pretty_string(const JsonValue *value);

#endif
