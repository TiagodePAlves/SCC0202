#ifndef __BUFFER_H__
#define __BUFFER_H__

#include <stdlib.h>
#include "utils.h"

typedef struct buffer buffer_t;

buffer_t *buf_alloc(size_t capacidade, size_t elem_tam);

buffer_t *buf_realloc(buffer_t *, size_t capacidade)
attribute(nonnull);

void buf_free(buffer_t *)
attribute(nonnull);

void *buf_acesso(buffer_t *, size_t posicao)
attribute(pure, nonnull);

#endif//__BUFFER_H__
