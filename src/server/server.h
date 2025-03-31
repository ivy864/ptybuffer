#ifndef PTYB_SERVER_INCLUDED
#define PTYB_SERVER_INCLUDED

#define __USE_MISC
#include <sys/un.h>
#include "../util/utils.h"


PtybServer *init_server(char *socket_domain);

/**
 * Initialize server and fork to new process.
 */
int start_server(char *domain);

#endif
