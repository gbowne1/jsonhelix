#include "json_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static const char *src;

static void skip_whitespace() {
    while (*src && isspace(*src)) src++;
}

static int match(const char *str) {
    skip_whitespace();
    size_t len = strlen(str);
    if (strncmp(src, str, len) == 0) {
        src += len;
        return 1;
    }
    return 0;
}

static JsonValue* parse_value();

static char* parse_string() {
    if (*src != '"') return NULL;
    src++; // skip opening quote
    const char *start = src;
    while (*src && *src != '"') {
        if (*src == '\\') src++; // skip escaped char
        src++;
    }
    if (*src != '"') return NULL;
    size_t len = src - start;
    char *str = (char *)malloc(len + 1);
    strncpy(str, start, len);
    str[len] = '\0';
    src++; // skip closing quote
    return str;
}

static double parse_number() {
    char *end;
    double val = strtod(src, &end);
    src = end;
    return val;
}

static JsonValue* parse_array() {
    if (*src != '[') return NULL;
    src++; skip_whitespace();

    JsonValue *array = (JsonValue *)calloc(1, sizeof(JsonValue));
    array->type = JSON_ARRAY;

    JsonValue *prev = NULL;
    while (*src && *src != ']') {
        JsonValue *item = parse_value();
        if (!item) break;
        if (!array->child)
            array->child = item;
        else
            prev->next = item;
        prev = item;

        skip_whitespace();
        if (*src == ',') src++;
        skip_whitespace();
    }

    if (*src == ']') src++;
    return array;
}

static JsonValue* parse_object() {
    if (*src != '{') return NULL;
    src++; skip_whitespace();

    JsonValue *object = (JsonValue *)calloc(1, sizeof(JsonValue));
    object->type = JSON_OBJECT;

    JsonValue *prev = NULL;
    while (*src && *src != '}') {
        char *key = parse_string();
        if (!key) return NULL;
        skip_whitespace();
        if (*src != ':') return NULL;
        src++; skip_whitespace();

        JsonValue *value = parse_value();
        if (!value) return NULL;
        value->key = key;

        if (!object->child)
            object->child = value;
        else
            prev->next = value;
        prev = value;

        skip_whitespace();
        if (*src == ',') src++;
        skip_whitespace();
    }

    if (*src == '}') src++;
    return object;
}

static JsonValue* parse_value() {
    skip_whitespace();

    if (*src == '"') {
        JsonValue *val = calloc(1, sizeof(JsonValue));
        val->type = JSON_STRING;
        val->string_value = parse_string();
        return val;
    }

    if (*src == '{') return parse_object();
    if (*src == '[') return parse_array();

    if (match("true")) {
        JsonValue *val = calloc(1, sizeof(JsonValue));
        val->type = JSON_BOOL;
        val->bool_value = 1;
        return val;
    }
    if (match("false")) {
        JsonValue *val = calloc(1, sizeof(JsonValue));
        val->type = JSON_BOOL;
        val->bool_value = 0;
        return val;
    }
    if (match("null")) {
        JsonValue *val = calloc(1, sizeof(JsonValue));
        val->type = JSON_NULL;
        return val;
    }

    if (isdigit(*src) || *src == '-') {
        JsonValue *val = calloc(1, sizeof(JsonValue));
        val->type = JSON_NUMBER;
        val->number_value = parse_number();
        return val;
    }

    return NULL;
}

JsonValue* json_parse(const char *input) {
    src = input;
    return parse_value();
}

void json_free(JsonValue *value) {
    if (!value) return;
    if (value->key) free(value->key);
    if (value->type == JSON_STRING && value->string_value)
        free(value->string_value);
    json_free(value->child);
    json_free(value->next);
    free(value);
}

void json_print(const JsonValue *value, int indent) {
    if (!value) return;

    for (int i = 0; i < indent; i++) putchar(' ');

    if (value->key)
        printf("\"%s\": ", value->key);

    switch (value->type) {
        case JSON_NULL: printf("null\n"); break;
        case JSON_BOOL: printf(value->bool_value ? "true\n" : "false\n"); break;
        case JSON_NUMBER: printf("%f\n", value->number_value); break;
        case JSON_STRING: printf("\"%s\"\n", value->string_value); break;
        case JSON_ARRAY:
            printf("[\n");
            json_print(value->child, indent + 2);
            for (const JsonValue *v = value->child->next; v; v = v->next)
                json_print(v, indent + 2);
            for (int i = 0; i < indent; i++) putchar(' ');
            printf("]\n");
            break;
        case JSON_OBJECT:
            printf("{\n");
            json_print(value->child, indent + 2);
            for (const JsonValue *v = value->child->next; v; v = v->next)
                json_print(v, indent + 2);
            for (int i = 0; i < indent; i++) putchar(' ');
            printf("}\n");
            break;
    }
}
