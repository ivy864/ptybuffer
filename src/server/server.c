#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "server.h"

#define __USE_MISC
#include <sys/un.h>

// alocate uninitialized server
PtybServer *server_alloc() {
    PtybServer *server = malloc(sizeof(PtybServer));
    server->addr = malloc(sizeof(struct sockaddr_un));

    return server;
}

/**
 * Free server and close socket.
 */
void server_free(PtybServer *server) {
    free(server->addr);
    close(server->sock);
    free(server);
}

int server_main(PtybServer *server) {
    char buffer [1024];

    listen(server->sock, 5);

    socklen_t addr_len = SUN_LEN(server->addr);

    while (1) {
        int connection = accept(server->sock, (struct sockaddr *) server->addr, &addr_len);

        if (connection < 0) {
            fprintf(stderr, "Error %d on connection()\n", errno);
        }

        while(read(connection, buffer, 1023) == 1023) {
            buffer[1023] = '\0';
            printf("%s", buffer);
        }

        printf("%s\n", buffer);

        close(connection);
    }

    return 2;
}

PtybServer *init_server(char *socket_domain) {
    PtybServer *server = server_alloc();

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

    if (bind(server->sock, (struct sockaddr *) &server->addr, SUN_LEN(server->addr)) < 0) {
        fprintf(stderr, "Error %d on bind()\n", errno);
        exit(EXIT_FAILURE);
    }

    return server;
}

int start_server(char *domain) {
    if (fork() == 0) {
        PtybServer *server = init_server(domain);
        exit(server_main(server));
    }

    return 0;
}
