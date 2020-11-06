#include "defs.h"


static inline
int digito_cmp(digito_t lhs, digito_t rhs) {
    return ((int) lhs) - ((int) rhs);
}

static
int num_cmp(const num_t *lhs, const num_t *rhs) {
    int cmp = 0;

    do {
        int res = digito_cmp(lhs->digito, rhs->digito);
        if (res != 0) {
            cmp = res;
        }

        lhs = lhs->prox;
        rhs = rhs->prox;
    } while (lhs != NULL && rhs != NULL);

    if (lhs == NULL && rhs == NULL) {
        return cmp;
    } else if (lhs == NULL) {
        return 1;
    } else {
        return -1;
    }
}

int bigint_cmp(const bigint_t *lhs, const bigint_t *rhs) {
    if (!lhs->neg && !rhs->neg) {
        return num_cmp(&lhs->numero, &rhs->numero);
    } else if (!lhs->neg) {
        return 1;
    } else if (!rhs->neg) {
        return -1;
    } else {
        return -num_cmp(&lhs->numero, &rhs->numero);
    }
}
