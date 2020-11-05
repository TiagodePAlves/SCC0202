#include "defs.h"


static
digito_t digito_add(digito_t lhs, digito_t rhs, digito_t *carry) {
    digito_t num = lhs + rhs + *carry;
    *carry = num / BASE;
    return num % BASE;
}

static
digito_t digito_sub(digito_t lhs, digito_t rhs, digito_t *carry) {
    digito_t num = ((BASE + lhs) - rhs) - *carry;

    if (num < BASE) {
        *carry = 1;
        return num;
    } else {
        *carry = 0;
        return num - BASE;
    }
}

static
num_t *num_aplica_op(
    num_t *lhs, const num_t *rhs,
    digito_t(*digito_op)(digito_t, digito_t, digito_t *),
    num_t *(*alloc_carry)(digito_t)
) {
    digito_t carry = 0;

    num_t *ant = NULL;
    num_t *zero = NULL;
    do {
        lhs->digito = digito_op(lhs->digito, rhs->digito, &carry);
        if (lhs->digito != 0) {
            zero = NULL;
        } else if (zero == NULL) {
            zero = ant;
        }

        ant = lhs;
        lhs = lhs->prox;
        rhs = rhs->prox;
    } while (rhs != NULL);

    while (carry != 0 && lhs != NULL) {
        lhs->digito = digito_op(lhs->digito, 0, &carry);

        ant = lhs;
        lhs = lhs->prox;
    }
    if (carry != 0) {
        ant->prox = alloc_carry(carry);
    }
    return zero;
}

static inline
void num_add(num_t *lhs, const num_t *rhs) {
    (void) num_aplica_op(lhs, rhs, digito_add, num_alloc);
}

static inline
void num_sub(num_t *lhs, const num_t *rhs) {
    num_t *zero = num_aplica_op(lhs, rhs, digito_sub, NULL);

    if (zero != NULL) {
        num_dealloc(zero->prox, true);
        zero->prox = NULL;
    }
}

bigint_t *bigint_add(const bigint_t *lhs, const bigint_t *rhs) {
    bigint_t *maior;
    const bigint_t *menor;

    int cmp = bigint_cmp(lhs, rhs);
    if (cmp == 0 && lhs->neg != rhs->neg) {
        return bigint_alloc();
    } else if (cmp < 0) {
        maior = bigint_copia(rhs);
        menor = lhs;
    } else {
        maior = bigint_copia(lhs);
        menor = rhs;
    }

    if (lhs->neg == rhs->neg) {
        num_add(&maior->numero, &menor->numero);
    } else {
        num_sub(&maior->numero, &menor->numero);
    }
    return maior;
}
