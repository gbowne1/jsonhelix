#ifndef BSON_H
#define BSON_H

#include "json_parser.h"
#include <stdint.h>

JsonValue* bson_parse(const uint8_t *data, size_t length);
uint8_t* bson_serialize(const JsonValue *value, size_t *out_len);

#endif
