#include "buffer.h"
#include <stdio.h>

struct buffer {
    size_t elem_tam;
    size_t capacidade;
    byte_t buf[];
};


static inline attribute(const)
// Tamanho
size_t buf_alloc_tam(size_t capacidade, size_t elem_tam) {
// os membros fixos podem ser calculados de duas maneiras
// https://en.wikipedia.org/wiki/Flexible_array_member#Effect_on_struct_size_and_padding
#ifndef offsetof
    // com sizeof, podem conter bytes de padding adicionais (improvavel nesse caso)
    size_t metadata = sizeof(struct buffer);
#else
    // mas o offsetof só é deifinido no <stddef.h>
    size_t metadata = offsetof(struct buffer, buf);
#endif
    // risco de overflow, ignorado
    size_t elementos = capacidade * elem_tam;
    return metadata + elementos;
}

buffer_t *buf_alloc(size_t capacidade, size_t elem_tam) {
    size_t size = buf_alloc_tam(capacidade, elem_tam);

    buffer_t *buf = malloc(size);
    ALLOC_CHECK(buf, buf_alloc, size);

    buf->capacidade = capacidade;
    buf->elem_tam = elem_tam;
    return buf;
}

attribute(nonnull)
buffer_t *buf_realloc(buffer_t *buffer, size_t capacidade) {
    size_t size = buf_alloc_tam(capacidade, buffer->elem_tam);

    buffer_t *new = realloc(buffer, size);
    ALLOC_CHECK(new, buf_realloc, size);

    new->capacidade = capacidade;
    return new;
}

attribute(nonnull)
void buf_free(buffer_t *buffer) {
    buffer->capacidade = 0;
    buffer->elem_tam = 0;
    free(buffer);
}

attribute(pure, nonnull)
void *buf_acesso(buffer_t *buffer, size_t posicao) {
    if (likely(posicao < buffer->capacidade)) {
        size_t byte_pos = posicao * buffer->elem_tam;
        return &buffer->buf[byte_pos];

    } else {
        return NULL;
    }
}
