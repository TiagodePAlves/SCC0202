#ifndef __RBTREE_H__
#define __RBTREE_H__

#include "utils.h"


struct rbtree {
    struct node *raiz;
};


struct rbtree rb_nova(void)
attribute(const, cold, leaf, nothrow);

void rb_dealloc(struct rbtree *tree)
attribute(nonnull, cold, leaf, nothrow);

#endif //__RBTREE_H__
