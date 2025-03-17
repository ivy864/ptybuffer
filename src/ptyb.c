#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./client/client.h"

int main(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "init") == 0) {
        return init_client();
    }
    return EXIT_SUCCESS;
}
