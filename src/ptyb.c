#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./client/client.h"
#include "./server/server.h"

int main(int argc, char *argv[])
{
    if (argc == 2) {
        if (strcmp(argv[1], "init") == 0) {
            server_connect("/home/horse/Repos/ptybuffer/sock");
            // return init_client();
        }
        else if (strcmp(argv[1], "start_server") == 0) {
            return init_server("/home/horse/Repos/ptybuffer/sock");
        }
    }

    return EXIT_SUCCESS;
}
