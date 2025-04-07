#define _XOPEN_SOURCE 700

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/select.h>

#define __USE_MISC
#include "client.h"
#include <termios.h>
#include <sys/ioctl.h>

int client_main(int master) {
    char input[1024];
    int rc = 0;
    printf("main started\n");

    fd_set fd_in;
    
    while (1) {
        FD_ZERO(&fd_in);
        FD_SET(0, &fd_in);
        FD_SET(master, &fd_in);

        // TODO: use that other function. see select man page
        rc = select(master + 1, &fd_in, NULL, NULL, NULL);

        if (rc == -1) {
            fprintf(stderr, "Error %d on select()\n%s\n", errno, strerror(errno));
            exit(1);
        }
        else {
            if (FD_ISSET(0, &fd_in)) {
                rc = read(0, input, sizeof(input));
                if (rc > 0) {
                    write(master, input, rc);
                    write(1, input, rc);
                    ptyb_message_server("/run/user/1000/ptyb.sock", input);
                }
                else {
                    if (rc < 0) {
                        fprintf(stderr, "Error %d on read stdin\n%s\n", errno, strerror(errno));
                        exit(1);
                    }
                }
            }

            if (FD_ISSET(master, &fd_in)) {
                rc = read(master, input, sizeof(input));
                if (rc > 0) {
                    write(1, input, rc);
                }
                else {
                    if (rc < 0) {
                        fprintf(stderr, "Error %d on read master PTY\n%s\n", errno, strerror(errno));
                        exit(1);
                    }
                }
            }
        }
    }


    return EXIT_SUCCESS;
}

int init_shell(int maid) {
    printf("???\n");
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

    close(maid);

    setsid();

    ioctl(0, TIOCSCTTY, 1);

    /* bash: cannot set terminal process group (5908): Inappropriate ioctl for
     * device bash: no job control in this shell */

    execlp("bash", "bash");

    return(EXIT_FAILURE);
}

/* code modified from https://www.rkoucha.fr/tech_corner/pty_pdip.html */

/* initialize stuff and fork to client_main? */
int init_client(char *sock_domain) {
    // 'maid' is used in place of 'slave' because I think slave is a weird name
    int master, maid, rc;
    uint32_t cid = ptyb_get_cid(sock_domain);
    printf("client recieved cid %d.\n", cid);

    master = posix_openpt(O_RDWR);
    if (master < 0) {
        fprintf(stderr, "Error %d on posix_openpt()\n", errno);
        return EXIT_FAILURE;
    }

    rc = grantpt(master);
    if (rc != 0) {
        fprintf(stderr, "Error %d on grantpt()\n", errno);
        return EXIT_FAILURE;
    }

    rc = unlockpt(master);
    if (rc != 0) {
        fprintf(stderr, "Error %d on unlockpt()\n", errno);
        return EXIT_FAILURE;
    }

    maid = open(ptsname(master), O_RDWR);

    if (fork()) {
        close(maid);
        client_main(master);
    }
    else {
        close(master);
        printf("%d\n", init_shell(maid));
        printf("huh\n");
    }

    exit(0);
}
