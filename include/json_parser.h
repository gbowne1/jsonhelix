#ifndef JSON_PARSER_H
#define JSON_PARSER_H

typedef enum {
    JSON_NULL,
    JSON_BOOL,
    JSON_NUMBER,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
} JsonType;

typedef struct JsonValue {
    JsonType type;
    char *key; // Optional (for object members)
    union {
        double number_value;
        char *string_value;
        int bool_value;
        struct JsonValue *child; // For objects/arrays
    };
    struct JsonValue *next; // Sibling node
} JsonValue;

JsonValue* json_parse(const char *input);
void json_free(JsonValue *value);
void json_print(const JsonValue *value, int indent);

#endif
