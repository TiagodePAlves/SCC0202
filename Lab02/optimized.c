// Optimized solution for the problem
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100000ULL

static bool expected[MAX] = {false};

static inline
unsigned read(void) {
    unsigned num;
    scanf("%u", &num);
    return num;
}

int main(void) {
    unsigned rounds = 0;

    for (unsigned N = read(); N > 0; N--) {
        unsigned i = read() - 1;

        if (!expected[i]) {
            rounds++;
        }
        expected[i+1] = true;
    }
    printf("%u\n", rounds);

    return EXIT_SUCCESS;
}
