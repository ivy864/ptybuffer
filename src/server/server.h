#ifndef PTYB_SERVER_INCLUDED
#define PTYB_SERVER_INCLUDED

#define __USE_MISC
#include <sys/un.h>
#include "../util/utils.h"

#define PTYB_BUFFER_CHUNK_SIZE 128 

PtybServer *init_server(char *socket_domain);

/**
 * Initialize server and fork to new process.
 */
int start_server(char *domain);

typedef struct PtybBuffer PtybBuffer;
struct PtybBuffer {
    char chunk[PTYB_BUFFER_CHUNK_SIZE];
    int32_t chunk_len;
    PtybBuffer *next;
    int32_t cid; // client ID
};

typedef struct PtybBufferList PtybBufferList ;
struct PtybBufferList {
    PtybBuffer *buffer;
    PtybBufferList *next;
};

PtybBufferList *ptyb_init_bufferlist();
PtybBuffer *ptyb_getbuffer(PtybBufferList *list, uint32_t cid);

int32_t ptyb_get_next_pid(PtybBufferList *list);

/*
 * initilize buffer and add it to list. return mutated list.
 */
PtybBufferList *ptyb_add_buffer(PtybBufferList *list, uint32_t cid);

PtybBuffer *ptyb_init_buffer(int cid);

/*
 * Insert string 'text' into buffer. 
 * The length of 'text' should not excede PTYB_BUFFER_CHUNK_SIZE
 *
 * returns -1 on error.
 */
int ptyb_buffer_insert(PtybBuffer *buffer, char *text);

int ptyb_buffer_free(PtybBuffer *buffer);

// message types
#define PTYBMSSG_CLIENT_ATTACH 1
#define PTYBMSSG_CLIENT_DETACH 2


#endif
