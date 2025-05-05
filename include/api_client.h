#ifndef API_CLIENT_H
#define API_CLIENT_H

#include "json_parser.h"

// Basic API Response struct
typedef struct {
    int status_code;
    char *body;
    JsonValue *json;
} ApiResponse;

ApiResponse* api_get(const char *host, const char *port, const char *path);
ApiResponse* api_post(const char *host, const char *port, const char *path, const char *json_payload);

void free_api_response(ApiResponse *resp);

#endif
