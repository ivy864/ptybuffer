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
    unlink(server->addr->sun_path);
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

int ptyb_format_buffer_html() {
    FILE *out = fopen("./formatted_buffer.html", "w");
    FILE *buffer = fopen("./buffer.txt", "r");
    fprintf(out, "<!DOCTYPE html><html>\n<head>\n<link rel='stylesheet' href='style.css' />\n");
    fprintf(out, "</head><body>\n<table>\n");

    char line[256];

    while (fgets(line, 256, buffer)) {
        fprintf(out, "<tr>\n");
        char *tok = strtok(line, " \t");

        while (tok != NULL) {
            fprintf(out, "<td>%s</td>\n", tok);
            tok = strtok(NULL, " \t");
        }

        fprintf(out, "</tr>\n");
    }
    fprintf(out, "</table></body></html>");

    fflush(out);

    fclose(out);
    fclose(buffer);
    return 0;
}
