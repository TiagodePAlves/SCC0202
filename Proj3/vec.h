#ifndef __VEC_H__
#define __VEC_H__

#include "utils.h"
#include <stdlib.h>


struct vec {
    size_t cap;
    size_t tam;
    size_t ini;
    void **data;
};

struct vec vec_init(size_t capacidade)
attribute(cold, leaf, nothrow);

void vec_dealloc(struct vec *vetor)
attribute(nonnull, cold, leaf, nothrow);

void vec_push_front(struct vec *restrict vetor, void *data)
attribute(nonnull, hot, leaf, nothrow);

void vec_push_back(struct vec *restrict vetor, void *data)
attribute(nonnull, hot, leaf, nothrow);;

void *vec_pop_front(struct vec *vetor)
attribute(nonnull, hot, leaf, nothrow);;

void *vec_pop_back(struct vec *vetor)
attribute(nonnull, hot, leaf, nothrow);;

#endif //__VEC_H__
