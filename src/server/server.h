#ifndef PTYB_SERVER_INCLUDED
#define PTYB_SERVER_INCLUDED

#define __USE_MISC
#include <sys/un.h>

typedef struct PtybServer PtybServer;

struct PtybServer {
    int sock; // socket file descriptor 
    struct sockaddr_un *addr; // socket address
};

PtybServer *init_server(char *socket_domain);

/**
 * Initialize server and fork to new process.
 */
int start_server(char *domain);

#endif
