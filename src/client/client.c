#define _XOPEN_SOURCE 700

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define __USE_MISC
#include <termios.h>
#include "client.h"

int client_main(int master) { 
    char input[1024];
    int rc = 0;

    while (1) {
        // use write instead of printf so it actually flushes
        // write(1, "Input: ", sizeof("Input: "));
        rc = read(0, input, sizeof(input));

        if (rc > 0) {
            write(master, input, rc);
            rc = read(master, input, sizeof(input) - 1);
            if (rc > 0) {
                input[rc] = '\0';

                fprintf(stderr, "%s", input);
            }
            else {
                break;
            }
        }
        else {
            break;
        }
    }
    return EXIT_SUCCESS; 
}

int init_shell(int maid) {
    struct termios maid_orig_term_settings;
    struct termios new_term_settings;

    // save default params of maid side of PTY
    tcgetattr(maid, &maid_orig_term_settings);

    // set raw mode of maid side of PTY
    new_term_settings = maid_orig_term_settings;
    cfmakeraw(&new_term_settings);
    tcsetattr(maid, TCSANOW, &new_term_settings);

    // close std streams, open pty as std streams
    close(0);
    close(1);
    close(2);

    dup(maid);
    dup(maid);
    dup(maid);

    execl("/bin/bash", "bash");
    /*
    while (1) {
        rc = read(maid, input, sizeof(input) - 1);

        if (rc > 0) {
            input[rc - 1] = '\0';
            printf("Child received: '%s'\n", input);
        }
        else {
            break;
        }
    }
    */
    return EXIT_SUCCESS;
}

/* code modified from https://www.rkoucha.fr/tech_corner/pty_pdip.html */

/* initialize stuff and fork to client_main? */
int init_client() {
    // 'maid' is used in place of 'slave' because I think slave is a weird name
    int master, maid, rc;

    master = posix_openpt(O_RDWR);
    if (master < 0) {
        fprintf(stderr, "Error %d on posix_openpt()\n", errno);
        return 1;
    }

    rc = grantpt(master);
    if (rc != 0) {
        fprintf(stderr, "Error %d on grantpt()\n", errno);
    }

    rc = unlockpt(master);
    if (rc != 0) {
        fprintf(stderr, "Error %d on unlockpt()\n", errno);
    }

    maid = open(ptsname(master), O_RDWR);

    if (fork()) {
        close(maid);
        client_main(master);
    }
    else {
        close(master);
        init_shell(maid);
    }

    return 0;
}
