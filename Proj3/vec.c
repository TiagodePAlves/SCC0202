#include "vec.h"
#include <string.h>


// Inicialização.
struct vec vec_init(size_t capacidade) {
    // capacidade não pode ser zero
    if (capacidade == 0) {
        capacidade = 1;
    }

    void **data = malloc(capacidade * sizeof(void *));
    // erro de alocação
    if unlikely(data == NULL) exit(EXIT_FAILURE);

    return (struct vec) {
        .cap = capacidade,
        .tam = 0,
        .ini = 0,
        .data = data
    };
}

// Destrói.
void vec_dealloc(struct vec *vetor) {
    // data nunca é nulo
    free(vetor->data);
}

static inline attribute(nonnull, cold, nothrow)
/**
 * Duplica a capacidade do vetor.
 */
void vec_aumenta(struct vec *vetor) {
    void **data = reallocarray(vetor->data, 2 * vetor->cap, sizeof(void *));
    // erro de alocação
    if unlikely(data == NULL) exit(EXIT_FAILURE);

    // move os elementos que atravessam o final do buffer
    if (vetor->ini + vetor->tam > vetor->cap) {
        size_t ini_tam = (vetor->ini + vetor->tam) - vetor->cap;
        memcpy(data + vetor->cap, data, ini_tam);
    }
    // novo buffer com maior capacidade
    vetor->cap *= 2;
    vetor->data = data;
}

// Insere no início.
void vec_push_front(struct vec *restrict vetor, void *dado) {
    if unlikely(vetor->tam == vetor->cap) {
        vec_aumenta(vetor);
    }
    // próxima posição livre
    size_t pos;
    if (vetor->ini > 0) {
        pos = --vetor->ini;
    } else {
        // vetor atravessa o final do buffer
        pos = vetor->ini = vetor->cap - 1;
    }
    // insere o elemento
    vetor->data[pos] = dado;
    vetor->tam += 1;
}

// Insere no final.
void vec_push_back(struct vec *restrict vetor, void *dado) {
    if unlikely(vetor->tam == vetor->cap) {
        vec_aumenta(vetor);
    }
    // próxima posição livre
    size_t pos = (vetor->ini + vetor->tam) % vetor->cap;
    // insere o elemento
    vetor->data[pos] = dado;
    vetor->tam += 1;
}

// Remove do final.
void *vec_pop_front(struct vec *vetor) {
    if unlikely(vetor->tam == 0) {
        return NULL;
    }
    // menor posição (considerando o inicio)
    size_t pos = vetor->ini;
    // mudança dos marcadores
    vetor->ini = (vetor->ini + 1) % vetor->cap;
    vetor->tam -= 1;
    // leitura do elemento
    return vetor->data[pos];
}

// Remove do início.
void *vec_pop_back(struct vec *vetor) {
    if unlikely(vetor->tam == 0) {
        return NULL;
    }

    // mudança dos marcadores
    vetor->tam -= 1;
    // maior posição (considerando o inicio)
    size_t pos = (vetor->ini + vetor->tam) % vetor->cap;
    // leitura do elemento
    return vetor->data[pos];
}
