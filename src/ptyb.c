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

    if (argc == 1) {
        return init_client(sock_domain);
    }
    if (argc >= 2) {
        if (strcmp(argv[1], "init") == 0) {
            return init_client(sock_domain);
        }
        else if (strcmp(argv[1], "start_server") == 0) {
            return start_server(sock_domain);
        }
        else if (strcmp(argv[1], "write") == 0) {
            ptyb_msg_write_buffer(sock_domain);

            if (fork() == 0) {
                // if an application is specified, use that. otherwise use default application for the given MIME type
                if (argc == 3) {
                    execlp(argv[2], argv[2], "./buffer.txt");
                }
                else {
                    system("xdg-open ./buffer.txt");
                }
            }
        }
        else {
            printf("Usage:\n\tptyb\t Start PTYBuffer\n\tptyb write [editor]\twrite buffer and open it in a text editor. If no editor is specified, file is opened using xdg-open.\n\tptyb start_server\tStart server without starting client.\n");
        }
    }

    return EXIT_SUCCESS;
}
