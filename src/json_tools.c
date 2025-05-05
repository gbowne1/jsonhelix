#include "json_tools.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int json_key_exists(const JsonValue *object, const char *key) {
    return find_json_key(object, key) != NULL;
}

const JsonValue* find_json_key(const JsonValue *object, const char *key) {
    if (!object || object->type != JSON_OBJECT) return NULL;
    const JsonValue *child = object->child;
    while (child) {
        if (child->key && strcmp(child->key, key) == 0) {
            return child;
        }
        child = child->next;
    }
    return NULL;
}

int json_array_length(const JsonValue *array) {
    if (!array || array->type != JSON_ARRAY) return 0;
    int count = 0;
    const JsonValue *cur = array->child;
    while (cur) {
        count++;
        cur = cur->next;
    }
    return count;
}

// --- Sorting helpers ---
static int compare_values(const JsonValue *a, const JsonValue *b, SortOrder order) {
    if (!a || !b) return 0;
    if (a->type != b->type) return 0;

    switch (a->type) {
        case JSON_STRING:
            return (order == SORT_ASCENDING)
                ? strcmp(a->string_value, b->string_value)
                : strcmp(b->string_value, a->string_value);
        case JSON_NUMBER:
            return (order == SORT_ASCENDING)
                ? (a->number_value > b->number_value) - (a->number_value < b->number_value)
                : (b->number_value > a->number_value) - (b->number_value < a->number_value);
        default:
            return 0;
    }
}

static JsonValue* sort_linked_list(JsonValue *head, SortOrder order) {
    if (!head || !head->next) return head;

    JsonValue *sorted = NULL;
    JsonValue *current = head;

    while (current) {
        JsonValue *next = current->next;

        if (!sorted || compare_values(current, sorted, order) < 0) {
            current->next = sorted;
            sorted = current;
        } else {
            JsonValue *s = sorted;
            while (s->next && compare_values(current, s->next, order) >= 0) {
                s = s->next;
            }
            current->next = s->next;
            s->next = current;
        }

        current = next;
    }

    return sorted;
}

void sort_json_object(JsonValue *object, SortOrder order) {
    if (!object || object->type != JSON_OBJECT || !object->child) return;
    // Sort by key alphabetically
    JsonValue *sorted = NULL, *cur = object->child;

    while (cur) {
        JsonValue *next = cur->next;

        if (!sorted || (order == SORT_ASCENDING
                        ? strcmp(cur->key, sorted->key) < 0
                        : strcmp(cur->key, sorted->key) > 0)) {
            cur->next = sorted;
            sorted = cur;
        } else {
            JsonValue *s = sorted;
            while (s->next && ((order == SORT_ASCENDING
                               ? strcmp(cur->key, s->next->key) >= 0
                               : strcmp(cur->key, s->next->key) <= 0))) {
                s = s->next;
            }
            cur->next = s->next;
            s->next = cur;
        }

        cur = next;
    }

    object->child = sorted;
}

void sort_json_array(JsonValue *array, SortOrder order) {
    if (!array || array->type != JSON_ARRAY || !array->child) return;
    array->child = sort_linked_list(array->child, order);
}

// --- Conversion Stubs ---

char* json_to_minified_string(const JsonValue *value) {
    // Placeholder for future implementation
    return strdup("{\"stub\":true}");
}

char* json_to_pretty_string(const JsonValue *value) {
    // Placeholder for future implementation
    return strdup("{\n  \"stub\": true\n}");
}
