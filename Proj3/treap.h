#ifndef __TREAP_H__
#define __TREAP_H__

#include "utils.h"

typedef uint64_t chave_t;
typedef uint64_t priority_t;

struct treap {
    struct node *raiz;
};

struct treap treap_nova(void)
attribute(const, cold, leaf, nothrow);

void treap_dealloc(struct treap *arvore)
attribute(nonnull, cold, leaf, nothrow);

bool treap_insere(struct treap *arvore, chave_t chave, priority_t prio)
attribute(nonnull, hot, leaf, nothrow);

bool treap_busca(const struct treap *arvore, chave_t chave)
attribute(pure, nonnull, hot, leaf, nothrow);

bool treap_remove(struct treap *arvore, chave_t chave)
attribute(nonnull, hot, leaf, nothrow);

enum ordem {
    PREORDEM,
    ORDEM,
    POSORDEM,
    LARGURA
};

void treap_percorre(const struct treap *arvore, void (*callback)(chave_t, priority_t), enum ordem ordem)
attribute(hot);

#endif //__TREAP_H__
