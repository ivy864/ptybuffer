#include "utils.h"
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

IntList *intlist_init(int32_t size) {
    IntList *newlist = malloc(sizeof(IntList));

    newlist->items = malloc(size * sizeof(uint64_t));
    newlist->size = size;
    newlist->len = 0;

    return newlist;
}

void intlist_free(IntList *list) {
    free(list->items);
    free(list);
}

uint64_t intlist_get(IntList *list, int32_t index) {
    if (index >= list->len || index < 0) {
        return INT32_MIN;
    }

    return *(list->items + index);
}

int intlist_set(IntList *list, uint64_t item, int32_t index) {
    if (index > list->len || index >= list->size || index < 0) {
        return -1;
    }

    list->items[index] = item;

    return 0;
}

int intlist_append(IntList *list, uint64_t item) {
    if (list->len == list->size + 1) {
        return -1;
    }

    list->items[list->len] = item;
    list->len += 1;

    return 0;
}
