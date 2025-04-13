#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ASSERT_EQ(v1, v2) ((v1 == v2)? printf("PASS! v1 == v2\n"): fprintf(stderr, "FAIL! v1 (%d) != v2 (%d)\n", v1, v2))

int main(int argc, char *argv[]) {
    PtybBuffer *buffer = ptyb_init_buffer(24);
    ASSERT_EQ(buffer->cid, 24);

    ptyb_buffer_insert(buffer, "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."); 
    ptyb_buffer_insert(buffer, "Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat");
    ptyb_buffer_insert(buffer, "Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.");
    ptyb_buffer_insert(buffer, "Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");

    PtybBuffer *b = buffer;

    printf("Buffer contents: ");
    while (b != NULL) {
        printf("%s", b->chunk);
        b = b->next;
    }
    printf("\n\n");

    printf("Expected: Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
    printf("Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat");
    printf("Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.");
    printf("Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
    printf("\n");
    /*
    Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequatDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
    Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequatDuis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
     */

    return EXIT_SUCCESS;
}
