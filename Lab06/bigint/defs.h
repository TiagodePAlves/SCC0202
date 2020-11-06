#ifndef __BIGINT_DEFS__
#define __BIGINT_DEFS__

#include "../bigint.h"
#include "../utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct num num_t;
typedef uint8_t digito_t;

#define BASE    ((digito_t) 10)
#define MAX     ((digito_t) 9)


struct num {
    digito_t digito;
    num_t *prox;
};

struct bigint {
    num_t numero;
    bool neg;
};


bigint_t *bigint_alloc(void);
num_t *num_alloc(digito_t digito);
void num_dealloc(num_t *num);

#endif //__BIGINT_DEFS__
