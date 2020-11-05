#include "defs.h"


#define U64_MASK ((uint128_t) UINT64_MAX)



static attribute(nonnull)
bool digito_incr(digit_t *digito) {
    if (digito->digito == UINT64_MAX) {
        digito->digito = 0;
        return false;
    } else {
        digito->digito += 1;
        return true;
    }
}

static attribute(nonnull)
void num_incr(digit_t *lhs) {
    digit_t *dig;
    for (dig = lhs; dig->prox != NULL; dig = dig->prox) {
        if (digito_incr(dig)) {
            return;
        }
    }
    if (!digito_incr(dig)) {
        dig->prox = digito_alloc(1);
    }
}

static attribute(nonnull)
void num_add(digit_t *lhs, const digit_t *rhs) {
    bool carry = false;

    digit_t *ant = NULL;
    do {
        uint128_t a = (uint128_t) lhs->digito;
        uint128_t b = (uint128_t) rhs->digito;
        uint128_t res = a + b + (uint128_t) carry;

        lhs->digito = (uint64_t) (res & U64_MASK);
        res >>= sizeof(uint64_t) * SIZE_BTYE;
        assert(res == 0 || res == 1);

        carry = (bool) res;
        if (!carry) {
            return;
        }

        ant = lhs;
        lhs = lhs->prox;
        rhs = rhs->prox;

    } while (lhs != NULL && rhs != NULL);

    if (lhs == NULL) {
        ant->prox = digito_copia(rhs, true);
    } else {
        num_incr(lhs);
    }
}

void bigint_add(bigint_t *lhs, const bigint_t *rhs) {
    if (lhs->neg == rhs->neg) {
        num_add(&lhs->numero, &rhs->numero);
    } else {

    }
}
