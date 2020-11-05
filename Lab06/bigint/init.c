#include "defs.h"


num_t *num_alloc(digito_t digito) {
    num_t *novo = malloc(sizeof(num_t));
    if (novo == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    novo->digito = digito;
    novo->prox = NULL;
    return novo;
}

num_t num_copia(const num_t *num) {
    num_t base = (num_t) {
        .digito = num->digito,
        .prox = NULL
    };

    num_t *ptr = &base;
    while (num->prox != NULL) {
        num = num->prox;
        ptr = ptr->prox = num_alloc(num->digito);
    }
    ptr->prox = num_alloc(num->digito);

    return base;
}

void num_dealloc(num_t *num, bool todos) {
    for (num_t *ptr = num; todos && ptr != NULL;) {
        num_t *prox = ptr->prox;
        free(ptr);
        ptr = prox;
    }
}

bigint_t *bigint_alloc(void) {
    bigint_t *novo = malloc(sizeof(bigint_t));
    if (novo == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    novo->numero = (num_t) {
        .digito = 0,
        .prox = NULL
    };
    novo->neg = false;

    return novo;
}

bigint_t *bigint_copia(const bigint_t *num) {
    bigint_t *novo = bigint_alloc();
    novo->numero = num_copia(&num->numero);
    novo->neg = num->neg;
    return novo;
}

void bigint_dealloc(bigint_t *num) {
    if (num->numero.prox != NULL) {
        num_dealloc(&num->numero, true);
    }
    free(num);
}
