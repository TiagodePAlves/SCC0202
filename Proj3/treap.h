#ifndef __TREAP_H__
/* Treap, árvore binária de busca com heap de máximo */
#define __TREAP_H__

#include "utils.h"


// Tipo das chaves armazenadas na árvore.
typedef uint64_t chave_t;
// Prioridade para o heap.
typedef uint64_t priority_t;

/**
 * Estrutura da árvore.
 */
struct treap {
    struct node *raiz;
};

/**
 * Construção de uma árvore vazia.
 */
struct treap treap_nova(void)
attribute(const, cold, leaf, nothrow);

/**
 * Desaloca a árvore e todos os seus nós.
 */
void treap_dealloc(struct treap *arvore)
attribute(nonnull, cold, leaf, nothrow);

/**
 * Insere nova chave na árvore, com a prioridade dada.
 *
 * Retorna `true` se o elemento existia na árvore.
 */
bool treap_insere(struct treap *arvore, chave_t chave, priority_t prio)
attribute(nonnull, hot, leaf, nothrow);

/**
 * Busca chave na árvore.
 *
 * Retorna `true` se o elemento existia na árvore.
 */
bool treap_busca(const struct treap *arvore, chave_t chave)
attribute(pure, nonnull, hot, leaf, nothrow);

/**
 * Remove uma chave da árvore.
 *
 * Retorna `true` se o elemento existia na árvore.
 */
bool treap_remove(struct treap *arvore, chave_t chave)
attribute(nonnull, hot, leaf, nothrow);

/**
 * Tipos de percurso na árvore.
 */
enum ordem {
    PREORDEM,
    ORDEM,
    POSORDEM,
    LARGURA
};

/**
 * Percorre a árvore, aplicando a função para cada chave.
 */
void treap_percorre(const struct treap *arvore, void (*callback)(chave_t, priority_t), enum ordem ordem)
attribute(hot);

#endif //__TREAP_H__
