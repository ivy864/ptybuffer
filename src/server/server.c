#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define __USE_MISC
#include <sys/un.h>

int init_server(char *socket_domain) {
    struct sockaddr_un addr;
    socklen_t addrlen;
    char buffer [1024];

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

    if (bind(sock, (struct sockaddr *) &addr, SUN_LEN(&addr)) < 0) {
        fprintf(stderr, "Error %d on bind()\n", errno);
        exit(EXIT_FAILURE);
    }

    addrlen = sizeof(addr.sun_path);

    listen(sock, 5);

    //while (1) {
        int connection = accept(sock, (struct sockaddr *) &addr, &addrlen);

        if (connection < 0) {
            fprintf(stderr, "Error %d on connection()\n", errno);
        }

        while(read(connection, buffer, 1023) == 1023) {
            buffer[1023] = '\0';
            printf("%s", buffer);
        }

        printf("%s\n", buffer);

        close(connection);
    //}

    return EXIT_SUCCESS; 
}
