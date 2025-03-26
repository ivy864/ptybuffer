#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

#define ASSERT_EQ(v1, v2) ((v1 == v2)? printf("PASS! v1 == v2\n"): fprintf(stderr, "FAIL! v1 != v2\n"))

int main(int argc, char *argv[]) {
    IntList *list = intlist_init(64);
    uint64_t testvalues[4] = {32, 22, 4124, 124};

    ASSERT_EQ(intlist_append(list, testvalues[0]), 0);
    ASSERT_EQ(intlist_append(list, testvalues[1]), 0);
    ASSERT_EQ(intlist_append(list, testvalues[2]), 0);
    ASSERT_EQ(intlist_set(list, testvalues[3], 3), 0);

    printf("get\n");
    for (int i = 0; i < 4; i++) {
        ASSERT_EQ(intlist_get(list, i), testvalues[i]);
        printf("num: %ld\n", intlist_get(list, i));
    }

    return EXIT_SUCCESS;
}
