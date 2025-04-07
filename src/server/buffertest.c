#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ASSERT_EQ(v1, v2) ((v1 == v2)? printf("PASS! v1 == v2\n"): fprintf(stderr, "FAIL! v1 (%d) != v2 (%d)\n", v1, v2))

int main(int argc, char *argv[]) {
    PtybBuffer *buffer = ptyb_init_buffer(24);
    ASSERT_EQ(buffer->cid, 24);

    ptyb_buffer_insert(buffer, "meow");
    ASSERT_EQ(strcmp("meow", buffer->chunk), 0);

    return EXIT_SUCCESS;
}
