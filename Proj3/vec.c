#include "vec.h"
#include <string.h>


struct vec vec_init(size_t capacidade) {
    void **data = malloc(capacidade * sizeof(void *));
    if unlikely(data == NULL) exit(EXIT_FAILURE);

    return (struct vec) {
        .cap = capacidade,
        .tam = 0,
        .ini = 0,
        .data = data
    };
}

void vec_dealloc(struct vec *vetor) {
    free(vetor->data);
}

static inline attribute(nonnull, cold, leaf, nothrow)
void vec_aumenta(struct vec *vetor) {
    void **data = reallocarray(vetor->data, 2 * vetor->cap, sizeof(void *));
    if unlikely(data == NULL) exit(EXIT_FAILURE);

    if (vetor->ini + vetor->tam > vetor->cap) {
        size_t ini_tam = (vetor->ini + vetor->tam) - vetor->cap;
        memcpy(vetor->data + vetor->cap, vetor->data, ini_tam);
    }
    vetor->cap *= 2;
    vetor->data = data;
}

void vec_push_front(struct vec *restrict vetor, void *data) {
    if unlikely(vetor->tam == vetor->cap) {
        vec_aumenta(vetor);
    }

    size_t pos;
    if (vetor->ini > 0) {
        pos = --vetor->ini;
    } else {
        pos = vetor->ini = vetor->cap - 1;
    }
    vetor->data[pos] = data;
    vetor->tam += 1;
}

void vec_push_back(struct vec *restrict vetor, void *data) {
    if unlikely(vetor->tam == vetor->cap) {
        vec_aumenta(vetor);
    }

    size_t pos = (vetor->ini + vetor->tam) % vetor->cap;

    vetor->data[pos] = data;
    vetor->tam += 1;
}

void *vec_pop_front(struct vec *vetor) {
    if unlikely(vetor->tam == 0) {
        return NULL;
    }

    size_t pos = vetor->ini;
    vetor->ini = (vetor->ini + 1) % vetor->cap;
    vetor->tam -= 1;
    return vetor->data[pos];
}

void *vec_pop_back(struct vec *vetor) {
    if unlikely(vetor->tam == 0) {
        return NULL;
    }

    vetor->tam -= 1;
    size_t pos = (vetor->ini + vetor->tam) % vetor->cap;
    return vetor->data[pos];
}
