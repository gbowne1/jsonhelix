#include "json_validator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const JsonValue* find_key(const JsonValue *obj, const char *key) {
    const JsonValue *child = obj->child;
    while (child) {
        if (child->key && strcmp(child->key, key) == 0) {
            return child;
        }
        child = child->next;
    }
    return NULL;
}

static void add_error(ValidationError **head, ValidatorResult code, const char *key, const char *msg) {
    ValidationError *err = (ValidationError *)malloc(sizeof(ValidationError));
    err->code = code;
    err->message = msg;
    err->key = key ? strdup(key) : NULL;
    err->next = *head;
    *head = err;
}

ValidationError* validate_json(const JsonValue *root, const SchemaField *schema, int schema_len) {
    ValidationError *errors = NULL;

    if (!root || root->type != JSON_OBJECT) {
        add_error(&errors, VALIDATOR_TYPE_MISMATCH, NULL, "Root JSON value is not an object.");
        return errors;
    }

    for (int i = 0; i < schema_len; i++) {
        const SchemaField *field = &schema[i];
        const JsonValue *value = find_key(root, field->key);

        if (!value) {
            if (field->required)
                add_error(&errors, VALIDATOR_MISSING_KEY, field->key, "Required key missing.");
            continue;
        }

        if (value->type != field->expected_type) {
            add_error(&errors, VALIDATOR_TYPE_MISMATCH, field->key, "Type mismatch.");
        }
    }

    return errors;
}

void free_validation_errors(ValidationError *err) {
    while (err) {
        ValidationError *next = err->next;
        if (err->key) free((char*)err->key);
        free(err);
        err = next;
    }
}

void print_validation_errors(const ValidationError *err) {
    while (err) {
        printf("Validation error [%d] on key \"%s\": %s\n",
               err->code, err->key ? err->key : "(root)", err->message);
        err = err->next;
    }
}
