#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "server.h"
#include "../util/utils.h"

#define __USE_MISC
#include <sys/un.h>

int server_main(PtybServer *server) {
    char buffer [1024];
    // number of connected clients
    uint32_t clients = 0;
    // id of connected client
    int32_t conid;
    uint32_t mtype;

    PtybBuffer *client_buffer = ptyb_init_buffer(0);

    listen(server->sock, 5);

    socklen_t addr_len = SUN_LEN(server->addr);

    while (1) {
        int connection = accept(server->sock, (struct sockaddr *) server->addr, &addr_len);

        if (connection < 0) {
            fprintf(stderr, "Error %d on connection()\n", errno);
        }

        read(connection, &conid, 4);
        /* -1 means a message has been sent
         * message format: 
         * 32 bits: message type
         * 32 bits: relevant client
         */
        if (conid == -1) {
            read(connection, &mtype, 4);
            read(connection, &conid, 4);
            
            switch (mtype) {
                case PTYBMSSG_CLIENT_ATTACH: 
                    send(connection, &clients, sizeof(clients), 0);
                    printf("client connected with number %d\n", clients);
                    clients += 1;
                    break;
                case PTYBMSSG_CLIENT_DETACH:
                    clients -= 1;
                    if (clients <= 0) {
                        ptybserver_free(server);
                        exit(0);
                    }
                    break;
                case PTYBMSSG_WRITE_BUFFER:
                    ptyb_write_buffer(client_buffer);
                    break;
            }
        }
        else {
            int bytes = read(connection, buffer, 1023);
            //ptyb_buffer_free(client_buffer);
            //client_buffer = ptyb_init_buffer(0);

            buffer[bytes] = '\0';
            ptyb_buffer_insert(client_buffer, buffer);
            
        }

        PtybBuffer *b = client_buffer;

        printf("Buffer contents: ");
        while (b != NULL) {
            printf("%s", b->chunk);
            b = b->next;
        }
        printf("\n\n");

        close(connection);
    }

    return 2;
}

PtybServer *init_server(char *socket_domain) {
    PtybServer *server = ptybserver_alloc();

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

    struct sockaddr *eek = (struct sockaddr *) server->addr;


// (struct sockaddr *) server->addr
    if (bind(server->sock, eek, SUN_LEN(server->addr)) < 0) {
        fprintf(stderr, "Error %d on bind()\n", errno);
        fprintf(stderr, "%s\n", strerror(errno));

        exit(EXIT_FAILURE);
    }

    return server;
}

int start_server(char *domain) {
    //if (fork() == 0) {
        PtybServer *server = init_server(domain);
        exit(server_main(server));
    //}

    return 0;
}
