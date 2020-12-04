#ifndef __RBTREE_H__
#define __RBTREE_H__

#include "utils.h"


struct rbtree {
    struct node *raiz;
};

typedef uint32_t chave_t;


struct rbtree rb_nova(void)
attribute(const, cold, leaf, nothrow);

void rb_dealloc(struct rbtree *tree)
attribute(nonnull, cold, leaf, nothrow);

bool rb_insere(struct rbtree *arvore, chave_t chave)
attribute(nonnull, leaf, cold, nothrow, access(read_write, 1));

bool rb_busca_succ(const struct rbtree *arvore, chave_t chave, chave_t *succ)
attribute(nonnull, leaf, cold, nothrow, access(read_only, 1), access(write_only, 3));

bool rb_busca_pred(const struct rbtree *arvore, chave_t chave, chave_t *pred)
attribute(nonnull, leaf, cold, nothrow, access(read_only, 1), access(write_only, 3));

bool rb_busca_max(const struct rbtree *arvore, chave_t *max)
attribute(nonnull, leaf, cold, nothrow, access(read_only, 1), access(write_only, 2));

bool rb_busca_min(const struct rbtree *arvore, chave_t *min)
attribute(nonnull, leaf, cold, nothrow, access(read_only, 1), access(write_only, 2));

void rb_pre_ordem(const struct rbtree *arvore, void (*callback)(chave_t chave, void *data), void *data)
attribute(nonnull(1), cold, access(read_only, 1));

void rb_em_ordem(const struct rbtree *arvore, void (*callback)(chave_t chave, void *data), void *data)
attribute(nonnull(1), cold, access(read_only, 1));

void rb_pos_ordem(const struct rbtree *arvore, void (*callback)(chave_t chave, void *data), void *data)
attribute(nonnull(1), cold, access(read_only, 1));

#endif //__RBTREE_H__
