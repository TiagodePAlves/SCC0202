#include "defs.h"


digito_t digito_add(digito_t lhs, digito_t rhs, digito_t *carry) {
    digito_t num = lhs + rhs + *carry;
    *carry = num / BASE;
    return num % BASE;
}

void num_add(num_t *lhs, const num_t *rhs) {
    digito_t carry = 0;

    num_t *ant;
    do {
        lhs->digito = digito_add(lhs->digito, rhs->digito, &carry);

        ant = lhs;
        lhs = lhs->prox;
        rhs = rhs->prox;
    } while (rhs != NULL);

    while (carry != 0 && lhs != NULL) {
        lhs->digito = digito_add(lhs->digito, 0, &carry);

        ant = lhs;
        lhs = lhs->prox;
    }
    if (carry != 0) {
        ant->prox = num_alloc(carry);
    }
}

bigint_t *bigint_add_positivo(const bigint_t *lhs, const bigint_t *rhs) {
    bigint_t *maior;
    const bigint_t *menor;

    if (bigint_cmp(lhs, rhs) < 0) {
        maior = bigint_copia(rhs);
        menor = lhs;
    } else {
        maior = bigint_copia(lhs);
        menor = rhs;
    }

    num_add(&maior->numero, &menor->numero);
    return maior;
}

bigint_t *bigint_add(const bigint_t *lhs, const bigint_t *rhs) {
    if (lhs->neg == rhs->neg) {
        return bigint_add_positivo(lhs, rhs);
    } else {
        exit(EXIT_FAILURE);
        return NULL;
    }
}
