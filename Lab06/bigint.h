#ifndef __BIGINT_H__
#define __BIGINT_H__

#include "utils.h"

typedef struct bigint bigint_t;


bigint_t *bigint_alloc(void);

bigint_t *bigint_copia(const bigint_t *num);

void bigint_dealloc(bigint_t *num);

int bigint_cmp(const bigint_t *lhs, const bigint_t *rhs);

bigint_t *bigint_add(const bigint_t *lhs, const bigint_t *rhs);

#endif //__BIGINT_H__
