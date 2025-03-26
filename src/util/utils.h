#ifndef PTYBUTIL_INCLUDED
#define PTYBUTIL_INCLUDED

#include <stddef.h>
#include <stdint.h>

typedef struct IntList IntList;

/**
 List of integers with bounds-checked accessors.
 */
struct IntList{
    int32_t size; /* allocated size of list */
    int32_t len; /* number of items curently in list */
    uint64_t *items;
};

/**
 * Initialize new IntList object
 *
 * @param size length of new list
 */
IntList *intlist_init(int32_t size);

/**
 * Free IntList
 */
void intlist_free(IntList *list);

/**
 * Get item from specified index 
 */
uint64_t intlist_get(IntList *list, int32_t index);

/**
 * Set value at position in list. Return 0 on success and -1 on failure.
 */
int intlist_set(IntList *list, uint64_t item, int32_t index);


/**
 * Append  item to list.
 * Set value at position in list. Return 0 on success and -1 on failure.
 */
int intlist_append(IntList *list, uint64_t item);
uint64_t intlist_resize(IntList *list, int32_t newlen);

#endif /* ifndef PTYBUTIL_INCLUDED */
