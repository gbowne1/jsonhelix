#include "api_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <errno.h>

#define BUFFER_SIZE 8192

static int connect_to_host(const char *host, const char *port) {
    struct addrinfo hints, *res, *p;
    int sockfd = -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, port, &hints, &res) != 0) return -1;

    for (p = res; p != NULL; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sockfd < 0) continue;
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == 0) break;
        close(sockfd);
        sockfd = -1;
    }

    freeaddrinfo(res);
    return sockfd;
}

static ApiResponse* send_http_request(const char *host, const char *port, const char *request) {
    int sockfd = connect_to_host(host, port);
    if (sockfd < 0) {
        perror("connect");
        return NULL;
    }

    send(sockfd, request, strlen(request), 0);

    char buffer[BUFFER_SIZE];
    int received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    if (received <= 0) {
        perror("recv");
        close(sockfd);
        return NULL;
    }

    buffer[received] = '\0';

    // Parse headers and body
    char *body = strstr(buffer, "\r\n\r\n");
    if (!body) {
        close(sockfd);
        return NULL;
    }

    body += 4; // skip past \r\n\r\n

    ApiResponse *resp = (ApiResponse *)calloc(1, sizeof(ApiResponse));
    resp->body = strdup(body);
    resp->status_code = 200; // TODO: parse real status from headers

    // Parse JSON body
    resp->json = json_parse(resp->body);

    close(sockfd);
    return resp;
}

ApiResponse* api_get(const char *host, const char *port, const char *path) {
    char request[1024];
    snprintf(request, sizeof(request),
             "GET %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-Agent: jsonhelix/1.0\r\n"
             "Connection: close\r\n\r\n",
             path, host);

    return send_http_request(host, port, request);
}

ApiResponse* api_post(const char *host, const char *port, const char *path, const char *json_payload) {
    char request[2048];
    snprintf(request, sizeof(request),
             "POST %s HTTP/1.1\r\n"
             "Host: %s\r\n"
             "User-Agent: jsonhelix/1.0\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %zu\r\n"
             "Connection: close\r\n\r\n"
             "%s",
             path, host, strlen(json_payload), json_payload);

    return send_http_request(host, port, request);
}

void free_api_response(ApiResponse *resp) {
    if (!resp) return;
    if (resp->body) free(resp->body);
    if (resp->json) json_free(resp->json);
    free(resp);
}
