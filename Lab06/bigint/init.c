#include "include.h"
#include <stdlib.h>


bigint_t *num_init_zero(void) {
    return calloc(1, sizeof(bigint_t));
}


void num_dealloc(bigint_t *num) {
    for (digit_t *ptr = num->number.prox; ptr != NULL;) {
        digit_t *prox = ptr->prox;

        ptr->digit = 0;
        ptr->prox = NULL;
        free(ptr);

        ptr = prox;
    }
    free(num);
}
