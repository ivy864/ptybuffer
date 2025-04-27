#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "../server/server.h"
#include "../util/utils.h"

#define __USE_MISC
#include <sys/un.h>

/*
 * Initialize server and establish connection. On error, return null.
 */
PtybServer *ptyb_client_connect(char *sock_domain) {
    PtybServer *server = ptybserver_alloc();

    ptybserver_sock_init(server, sock_domain);

    if (connect(server->sock, (struct sockaddr *) server->addr, SUN_LEN(server->addr)) < 0) {
        /* returning null is perhaps a bit dangerous, but this can't be a 
         * cancellation point since ptyb_get_cid(char *) needs to be able to 
         * try to start the server and connect again. */
        return NULL;
    }

    return server;
}

int ptyb_message_server(char *sock_domain, char *message) {
    int m = 1;
    PtybServer *server = ptyb_client_connect(sock_domain);

    if (server == NULL) {
        fprintf(stderr, "Error %d on connect()\n%s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    send(server->sock, &m, sizeof(m), 0);

    send(server->sock, message, strlen(message), 0);
    close(server->sock);

    return 0;
}

int ptyb_get_cid(char *sock_domain) {
    PtybServer *server = ptyb_client_connect(sock_domain);
    if (server == NULL) {
        printf("Connection failed!\n");
        // errno 2 = file not found
        // this means the server likely is not running. Start it and try again.
        if (errno == 2) {
            printf("trying again...\n");

            if (fork() == 0) {
                // close stdout so nothing gets printed
                close(1);
                start_server(sock_domain);
            }
            // make sure server actually starts
            sleep(1);
            server = ptyb_client_connect(sock_domain);
            if (server == NULL) {
                // I've heard goto is bad but this feels nicer than code repetition
                goto server_failure;
            }
        }
        else {
            goto server_failure;
        }
    }

    int32_t m = -1;
    uint32_t cid = 0;

    send(server->sock, &m, sizeof(m), 0);
    m = PTYBMSSG_CLIENT_ATTACH;
    send(server->sock, &m, sizeof(m), 0);
    m = -1;
    send(server->sock, &m, sizeof(m), 0);

    read(server->sock, &cid, sizeof(cid));

    return cid;

server_failure:
    fprintf(stderr, "Error %d on connect()\n%s\n", errno, strerror(errno));
    exit(EXIT_FAILURE);
}

void ptyb_msg_write_buffer(char *sock_domain) {
    PtybServer *server = ptyb_client_connect(sock_domain);
    if (server == NULL) {
        fprintf(stderr, "Error %d on connect()\n%s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    int32_t m = -1;

    send(server->sock, &m, sizeof(m), 0);
    m = PTYBMSSG_WRITE_BUFFER;
    send(server->sock, &m, sizeof(m), 0);
    m = 0;
    send(server->sock, &m, sizeof(m), 0);
}

void ptyb_msg_client_closed(char *sock_domain) {
    PtybServer *server = ptyb_client_connect(sock_domain);
    // Can't connect to server -- It's probably already closed.
    if (server == NULL) {
        return;
    }

    int32_t m = -1;

    send(server->sock, &m, sizeof(m), 0);
    m = PTYBMSSG_CLIENT_DETACH;
    send(server->sock, &m, sizeof(m), 0);
    m = 0;
    send(server->sock, &m, sizeof(m), 0);
}
