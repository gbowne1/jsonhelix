#include "bson.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define BSON_DOUBLE      0x01
#define BSON_STRING      0x02
#define BSON_OBJECT      0x03
#define BSON_ARRAY       0x04
#define BSON_BOOL        0x08
#define BSON_NULL        0x0A
#define BSON_INT32       0x10
#define BSON_INT64       0x12

static const uint8_t* parse_cstring(const uint8_t *ptr, char **out) {
    const uint8_t *start = ptr;
    while (*ptr != 0) ptr++;
    size_t len = ptr - start;
    *out = strndup((const char*)start, len);
    return ptr + 1;
}

static const uint8_t* parse_string(const uint8_t *ptr, char **out) {
    int32_t len = *(int32_t*)ptr;
    ptr += 4;
    *out = strndup((const char*)ptr, len - 1);
    return ptr + len;
}

static const uint8_t* parse_element(const uint8_t *ptr, JsonValue **value);

static const uint8_t* parse_document(const uint8_t *ptr, JsonValue *container, int is_array) {
    int32_t size = *(int32_t*)ptr;
    ptr += 4;

    while (*ptr != 0) {
        uint8_t type = *ptr++;
        char *key;
        ptr = parse_cstring(ptr, &key);

        JsonValue *val = NULL;
        ptr = parse_element(ptr - 1, &val); // include type byte again

        if (val) {
            if (!is_array)
                val->key = key;
            else
                free(key); // array index keys not used

            val->next = container->child;
            container->child = val;
        }
    }

    return ptr + 1;
}

static const uint8_t* parse_element(const uint8_t *ptr, JsonValue **value) {
    uint8_t type = *ptr++;
    char *key;
    ptr = parse_cstring(ptr, &key); // read key, but may discard if array

    JsonValue *val = calloc(1, sizeof(JsonValue));

    switch (type) {
        case BSON_DOUBLE:
            val->type = JSON_NUMBER;
            val->number_value = *(double*)ptr;
            ptr += 8;
            break;
        case BSON_STRING:
            val->type = JSON_STRING;
            ptr = parse_string(ptr, &val->string_value);
            break;
        case BSON_BOOL:
            val->type = JSON_BOOL;
            val->bool_value = *ptr++;
            break;
        case BSON_NULL:
            val->type = JSON_NULL;
            break;
        case BSON_OBJECT:
            val->type = JSON_OBJECT;
            ptr = parse_document(ptr, val, 0);
            break;
        case BSON_ARRAY:
            val->type = JSON_ARRAY;
            ptr = parse_document(ptr, val, 1);
            break;
        case BSON_INT32:
            val->type = JSON_NUMBER;
            val->number_value = *(int32_t*)ptr;
            ptr += 4;
            break;
        case BSON_INT64:
            val->type = JSON_NUMBER;
            val->number_value = *(int64_t*)ptr;
            ptr += 8;
            break;
        default:
            fprintf(stderr, "Unsupported BSON type: 0x%02x\n", type);
            free(val);
            *value = NULL;
            return ptr;
    }

    *value = val;
    free(key);
    return ptr;
}

JsonValue* bson_parse(const uint8_t *data, size_t length) {
    if (length < 5) return NULL;
    JsonValue *root = calloc(1, sizeof(JsonValue));
    root->type = JSON_OBJECT;
    parse_document(data, root, 0);
    return root;
}

uint8_t* bson_serialize(const JsonValue *value, size_t *out_len) {
    // Stub: Implementing full serializer is complex
    // For now just return NULL and set length to 0
    *out_len = 0;
    return NULL;
}
