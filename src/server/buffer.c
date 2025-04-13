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
    int insrtlen;
    insrtlen = strlen(text);

    if (insrtlen > PTYB_BUFFER_CHUNK_SIZE) {
        return -1;
    }

    while (PTYB_BUFFER_CHUNK_SIZE - buffer->chunk_len == 1) {
        buffer = buffer->next;
    }

    /* max chunk len - buffer->chunk len */
    int catbytes = PTYB_BUFFER_CHUNK_SIZE - buffer->chunk_len - 1;

    strncat(buffer->chunk, text, catbytes);
    buffer->chunk_len = strlen(buffer->chunk);

    insrtlen -= catbytes;
    if (insrtlen > 0) {
        buffer->next = ptyb_init_buffer(buffer->cid);
        buffer = buffer->next;

        //text = text + catbytes;

        strncat(buffer->chunk, text + catbytes, insrtlen);
        buffer->chunk_len = strlen(buffer->chunk);
    }
    
    return 0;
}

void ptyb_buffer_free(PtybBuffer *buffer) {
    if (buffer == NULL) {
        return;
    }

    PtybBuffer *next = buffer->next;

    while(buffer != NULL) {
        next = buffer->next;
        free(buffer);
        buffer = next;
    }
}
