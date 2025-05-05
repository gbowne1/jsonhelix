#ifndef JSON_VALIDATOR_H
#define JSON_VALIDATOR_H

#include "json_parser.h"

typedef enum {
    VALIDATOR_OK,
    VALIDATOR_MISSING_KEY,
    VALIDATOR_TYPE_MISMATCH,
    VALIDATOR_UNKNOWN_KEY,
    VALIDATOR_INVALID_VALUE
} ValidatorResult;

typedef struct ValidationError {
    ValidatorResult code;
    const char *message;
    const char *key;
    struct ValidationError *next;
} ValidationError;

typedef struct {
    const char *key;
    JsonType expected_type;
    int required;
} SchemaField;

ValidationError* validate_json(const JsonValue *root, const SchemaField *schema, int schema_len);
void free_validation_errors(ValidationError *err);
void print_validation_errors(const ValidationError *err);

#endif
