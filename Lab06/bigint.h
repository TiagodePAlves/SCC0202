#ifndef __BIGINT_H__
#define __BIGINT_H__

#include <stdio.h>

typedef struct bigint bigint_t;


bigint_t *bigint_alloc(void);

bigint_t *bigint_copia(const bigint_t *num);

void bigint_dealloc(bigint_t *num);

int bigint_cmp(const bigint_t *lhs, const bigint_t *rhs);

bigint_t *bigint_add(const bigint_t *lhs, const bigint_t *rhs);

bigint_t *bigint_read(FILE *stream);

void bigint_show(FILE *stream, const bigint_t *num);

#endif //__BIGINT_H__
