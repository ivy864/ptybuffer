#include "server.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

int ptyb_write_buffer(PtybBuffer *buffer) {
    if (buffer == NULL) {
        return -1;
    }

    FILE *out = fopen("./buffer.txt", "w");
    // flag for if buffer ended in the midle of an escape sequence

    while (buffer != NULL) {
        fprintf(out, "%s", buffer->chunk);
        buffer = buffer->next;
    }

    fflush(out);
    fclose(out);

    // strip control codes and other unwanted things using sed
    // use of system here is probably a security issue. 

    // remove ANSI escape sequences
    system("sed -i 's/\x1b\[[0-9;]*m//g' ./buffer.txt");
    // remove xterm control sequence for setting window title
    system("sed -i 's/.*\\x07//g' ./buffer.txt");
    // I don't actually know what 004l<CR> is but this removes it
    system("sed -i 's/004l\\x0d//g' ./buffer.txt");

    return 0;
}
