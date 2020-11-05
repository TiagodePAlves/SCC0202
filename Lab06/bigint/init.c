#include "defs.h"


bigint_t *num_init_zero(void) {
    bigint_t *novo = calloc(1, sizeof(bigint_t));
    if (novo == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    return novo;
}

digit_t *num_alloc_digito(uint64_t digit) {
    digit_t *novo = calloc(1, sizeof(digit_t));
    if unlikely(novo == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    novo->digito = digit;
    return novo;
}

void num_dealloc(bigint_t *num) {
    for (digit_t *ptr = num->numero.prox; ptr != NULL;) {
        digit_t *prox = ptr->prox;

        ptr->digito = 0;
        ptr->prox = NULL;
        free(ptr);

        ptr = prox;
    }
    free(num);
}

static inline
uint64_t copia_incr_digito(const digit_t *digito, bool *incr) {
    if (*incr) {
        if (digito->digito == UINT64_MAX) {
            return 0;
        } else {
            *incr = false;
            return digito->digito + 1;
        }
    } else {
        return digito->digito;
    }
}

digit_t *num_digito_copia(const digit_t *digito, bool incr) {
    digit_t *copia = num_alloc_digito(copia_incr_digito(digito, &incr));

    digito = digito->prox;
    digit_t *ptr;
    for (ptr = copia; digito != NULL; digito = digito->prox) {
        ptr = ptr->prox = num_alloc_digito(copia_incr_digito(digito, &incr));
    }

    if (incr) {
        ptr->prox = num_alloc_digito(1);
    }
    return copia;
}
