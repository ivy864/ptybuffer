#ifndef PTYBUTIL_INCLUDED
#define PTYBUTIL_INCLUDED

#include <stddef.h>
#include <stdint.h>

typedef struct PtybServer PtybServer;

struct PtybServer {
    int sock; // socket file descriptor 
    struct sockaddr_un *addr; // socket address
};

/**
 * Allocate memmory for PtybServer object
 */
PtybServer *ptybserver_alloc();

/**
 * Free PtybServer
 */
void ptybserver_free(PtybServer *server);

/**
 * Open socket in at sock_domain.
 */
void ptybserver_sock_init(PtybServer *server, char *sock_domain);

#endif /* ifndef PTYBUTIL_INCLUDED */
