#ifndef __TREAP_H__
#define __TREAP_H__

#include "utils.h"

typedef uint64_t chave_t;
typedef uint64_t priority_t;

struct treap {
    struct node *raiz;
};

struct treap treap_nova(void);

void treap_dealloc(struct treap *arvore);

int treap_insere(struct treap *arvore, chave_t chave, priority_t prio);

bool treap_busca(const struct treap *arvore, chave_t chave);

int treap_remove(struct treap *arvore, chave_t chave);

#endif //__TREAP_H__
