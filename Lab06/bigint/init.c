#include "defs.h"


bigint_t *bigint_init(void) {
    bigint_t *novo = calloc(1, sizeof(bigint_t));
    if (novo == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    return novo;
}

void bigint_dealloc(bigint_t *num) {
    for (digit_t *ptr = num->numero.prox; ptr != NULL;) {
        digit_t *prox = ptr->prox;

        ptr->digito = 0;
        ptr->prox = NULL;
        free(ptr);

        ptr = prox;
    }
    free(num);
}

bigint_t *bigint_copia(const bigint_t *num) {
    bigint_t *novo = bigint_init();

    novo->numero.digito = num->numero.digito;
    novo->numero.prox = digito_copia(num->numero.prox, false);
    novo->neg = num->neg;

    return novo;
}


digit_t *digito_alloc(uint64_t digit) {
    digit_t *novo = calloc(1, sizeof(digit_t));
    if (novo == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    novo->digito = digit;
    return novo;
}

static inline
uint64_t acessa_digito_incr(const digit_t *digito, bool *incr) {
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

digit_t *digito_copia(const digit_t *digito, bool incr) {
    if (digito == NULL) {
        if (incr) {
            return digito_alloc(1);
        } else {
            return NULL;
        }
    }

    digit_t *copia = digito_alloc(acessa_digito_incr(digito, &incr));

    digito = digito->prox;
    digit_t *ptr;
    for (ptr = copia; digito != NULL; digito = digito->prox) {
        ptr = ptr->prox = digito_alloc(acessa_digito_incr(digito, &incr));
    }

    if (incr) {
        ptr->prox = digito_alloc(1);
    }
    return copia;
}
