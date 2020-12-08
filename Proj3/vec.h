#ifndef __VEC_H__
/* Vetor geral, que pode servir como pilha ou fila. */
#define __VEC_H__

#include "utils.h"
#include <stdlib.h>


/**
 * Estrutura do vetor.
 */
struct vec {
    // Capacidade
    size_t cap;
    // Tamanho
    size_t tam;
    // Posição inicial
    size_t ini;
    // Dados
    void **data;
};

/**
 * Inicialização do vetor, com uma capacidade incial.
 */
struct vec vec_init(size_t capacidade)
attribute(cold, leaf, nothrow);

/**
 * Destrói um vetor, sem desalocar os elementos.
 *
 * Vetor deveria estar vazio ou a memória dos elementos
 * deve ser  controlada por outra estrutura.
 */
void vec_dealloc(struct vec *vetor)
attribute(nonnull, cold, leaf, nothrow);

/**
 * Insere no começo do vetor (menor posição).
 *
 * `dado` não deveria ser NULL.
 */
void vec_push_front(struct vec *restrict vetor, void *dado)
attribute(nonnull, hot, leaf, nothrow);

/**
 * Insere no final do vetor (maior posição).
 *
 * `dado` não deveria ser NULL.
 */
void vec_push_back(struct vec *restrict vetor, void *dado)
attribute(nonnull, hot, leaf, nothrow);

/**
 * Remove do começo do vetor (menor posição).
 *
 * Retorna NULL se estiver vazio.
 */
void *vec_pop_front(struct vec *vetor)
attribute(nonnull, hot, leaf, nothrow);

/**
 * Remove do final do vetor (maior posição).
 *
 * Retorna NULL se estiver vazio.
 */
void *vec_pop_back(struct vec *vetor)
attribute(nonnull, hot, leaf, nothrow);

#endif //__VEC_H__
