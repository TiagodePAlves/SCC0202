#ifndef __BIGINT_DEFS__
#define __BIGINT_DEFS__

#include "../bigint.h"

typedef struct digit digit_t;

struct digit {
    uint64_t digit;
    digit_t *prox;
};

struct bigint {
    digit_t number;
    bool neg;
};


#endif //__BIGINT_DEFS__