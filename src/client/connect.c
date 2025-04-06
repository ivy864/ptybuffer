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

PtybServer *ptyb_client_connect(char *sock_domain) {
    PtybServer *server = ptybserver_alloc();

    ptybserver_sock_init(server, sock_domain);

    if (connect(server->sock, (struct sockaddr *) server->addr, SUN_LEN(server->addr)) < 0) {
        fprintf(stderr, "Error %d on connect()\n%s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return server;
}

int ptyb_message_server(char *sock_domain, char *message) {
    PtybServer *server = ptyb_client_connect(sock_domain);

    send(server->sock, message, strlen(message), 0);
    close(server->sock);

    return 0;
}

int ptyb_get_cid(char *sock_domain) {
    PtybServer *server = ptyb_client_connect(sock_domain);
    int32_t m = -1;
    uint32_t cid = 0;

    send(server->sock, &m, sizeof(m), 0);
    m = PTYBMSSG_CLIENT_ATTACH;
    send(server->sock, &m, sizeof(m), 0);
    m = -1;
    send(server->sock, &m, sizeof(m), 0);

    read(server->sock, &cid, sizeof(cid));

    return cid;
}

int server_connect(char *socket_domain) {
    struct sockaddr_un addr;
    int sock = socket(AF_LOCAL, SOCK_STREAM, 0);

    if (sock < 0) {
        fprintf(stderr, "Error %d on socket()\n", errno);
        exit(EXIT_FAILURE);
    }

    if (strlen(socket_domain) > sizeof(addr.sun_path) - 1) {
        fprintf(stderr, "Server socket path too long.");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_domain, sizeof(addr.sun_path));

    if (connect(sock, (struct sockaddr *) &addr, SUN_LEN(&addr)) < 0) {
        fprintf(stderr, "Error %d on connect()\n%s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    send(sock, "meow\n", strlen("meow"), 0);
    close(sock);

    return 0;
}
