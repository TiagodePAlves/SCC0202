#ifndef __BIGINT_DEFS__
#define __BIGINT_DEFS__

#include "../bigint.h"
#include "../utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct digit digit_t;

struct digit {
    uint64_t digito;
    digit_t *prox;
};

struct bigint {
    digit_t numero;
    bool neg;
};

digit_t *digito_alloc(uint64_t digit);

digit_t *digito_copia(const digit_t *digito, bool incr);

#endif //__BIGINT_DEFS__