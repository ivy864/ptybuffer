#include "server.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

PtybBufferList *ptyb_init_bufferlist() {
    PtybBufferList *list = malloc(sizeof(PtybBufferList));

    list->buffer = NULL;
    list->next = NULL;

    return list;
}

PtybBuffer *ptyb_init_buffer(int32_t cid) {
    PtybBuffer *buffer = malloc(sizeof(PtybBuffer));

    memset(buffer, 0, sizeof(PtybBuffer));

    buffer->cid = cid;
    buffer->chunk_len = 0;

    return buffer;
}

int32_t ptyb_get_next_pid(PtybBufferList *list) {
    int32_t pid = 0;

    if (list == NULL || list->buffer == NULL) {
        return -1;
    }

    /* list->next == null
     * list->buffer == null
     */



    return pid;
}

PtybBufferList *ptyb_add_buffer(PtybBufferList *list, uint32_t cid) {

    return list;
}

int ptyb_buffer_insert(PtybBuffer *buffer, char *text) {
    /*
    int insrtlen;
    insrtlen = strlen(text);
    */

    while (PTYB_BUFFER_CHUNK_SIZE - buffer->chunk_len == 1) {
        buffer = buffer->next;
    }

    /* max chunk len - buffer->chunk len
     */

    strncat(buffer->chunk, text, PTYB_BUFFER_CHUNK_SIZE - buffer->chunk_len - 1);

    return 0;
}
