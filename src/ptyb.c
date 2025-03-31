#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./client/client.h"
#include "./server/server.h"
#include "./util/utils.h"
#include <stdint.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    uint32_t uid = getuid();
    char sock_domain[108];
    snprintf(sock_domain, 108, "/run/user/%d/ptyb.sock", uid);

    if (argc == 2) {
        if (strcmp(argv[1], "init") == 0) {
            //PtybServer *server =  ptybserver_alloc();
            //ptybserver_sock_init(server, sock_domain);
            //server_connect("/home/horse/Repos/ptybuffer/sock");
            //server_connect(sock_domain);
            return init_client();
        }
        else if (strcmp(argv[1], "start_server") == 0) {
            return start_server(sock_domain);
        }
    }

    return EXIT_SUCCESS;
}
