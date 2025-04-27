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

void ptyb_msg_write_buffer(char *sock_domain) {
    PtybServer *server = ptyb_client_connect(sock_domain);
    int32_t m = -1;

    send(server->sock, &m, sizeof(m), 0);
    m = PTYBMSSG_WRITE_BUFFER;
    send(server->sock, &m, sizeof(m), 0);
    m = 0;
    send(server->sock, &m, sizeof(m), 0);
}

void ptyb_msg_client_closed(char *sock_domain) {
    PtybServer *server = ptyb_client_connect(sock_domain);
    int32_t m = -1;

    send(server->sock, &m, sizeof(m), 0);
    m = PTYBMSSG_CLIENT_DETACH;
    send(server->sock, &m, sizeof(m), 0);
    m = 0;
    send(server->sock, &m, sizeof(m), 0);

}
