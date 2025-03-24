#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define __USE_MISC
#include <sys/un.h>

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
