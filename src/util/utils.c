#include "utils.h"
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>

#define __USE_MISC
#include <sys/un.h>

PtybServer *ptybserver_alloc() {
    PtybServer *server = malloc(sizeof(PtybServer));
    server->addr = malloc(sizeof(struct sockaddr_un));

    return server;
}

void ptybserver_free(PtybServer *server) {
    free(server->addr);
    close(server->sock);
    free(server);
}

void ptybserver_sock_init(PtybServer *server, char *socket_domain) {
    server->sock = socket(AF_LOCAL, SOCK_STREAM, 0);

    if (server->sock < 0) {
        fprintf(stderr, "Error %d on socket()\n", errno);
        exit(EXIT_FAILURE);
    }

    if (strlen(socket_domain) > sizeof(server->addr->sun_path) - 1) {
        fprintf(stderr, "Server socket path too long.");
        exit(EXIT_FAILURE);
    }

    memset(server->addr, 0, sizeof(struct sockaddr_un));
    server->addr->sun_family = AF_UNIX;
    strncpy(server->addr->sun_path, socket_domain, sizeof(server->addr->sun_path));

}

IntList *intlist_init(int32_t size) {
    IntList *newlist = malloc(sizeof(IntList));

    newlist->items = malloc(size * sizeof(uint64_t));
    newlist->size = size;
    newlist->len = 0;

    return newlist;
}

void intlist_free(IntList *list) {
    free(list->items);
    free(list);
}

uint64_t intlist_get(IntList *list, int32_t index) {
    if (index >= list->len || index < 0) {
        return INT32_MIN;
    }

    return *(list->items + index);
}

int intlist_set(IntList *list, uint64_t item, int32_t index) {
    if (index > list->len || index >= list->size || index < 0) {
        return -1;
    }

    list->items[index] = item;

    return 0;
}

int intlist_append(IntList *list, uint64_t item) {
    if (list->len == list->size + 1) {
        return -1;
    }

    list->items[list->len] = item;
    list->len += 1;

    return 0;
}
