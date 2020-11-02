#include <stdlib.h>
#include <stdio.h>
#include "random.h"


int main(int argc, const char *argv[]) {
    size_t count = 10;
    if (argc >= 2) {
        (void) sscanf(argv[1], "%zu", &count);
    }

    for (size_t i = 0; i < count; i++) {
        (void) printf("%hhu\n", rand_hl());
    }
    return EXIT_SUCCESS;
}
