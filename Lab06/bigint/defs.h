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

digit_t *num_alloc_digito(uint64_t digit);

digit_t *num_digito_copia(const digit_t *digito, bool incr)
attribute(nonull);

#endif //__BIGINT_DEFS__