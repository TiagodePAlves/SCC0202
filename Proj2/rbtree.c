#include "rbtree.h"
#include <stdlib.h>

/**
 * Propriedades:
 *  1 - Raiz é preta.
 *  2 - Toda folha (NULL) é preta.
 *  3 - Se um nó é vermelhos, seus filhos são pretos.
 *  4 - Partindo de um nó qualquer, todos os caminhos até
 *      as folhas contêm o mesmo número de nós pretos.
 */

typedef struct node attribute(aligned(4)) node_t;

typedef enum color {
    BLACK = 0,
    RED
} color_t;

struct node {
    color_t color;
    uint32_t chave;
    node_t *esq, *dir;
} attribute(aligned(4));

_Static_assert(sizeof(color_t) == 4, "");
_Static_assert(sizeof(node_t) == 24, "");
_Static_assert(sizeof(node_t *) == 8, "");

// TODO: leaf, hot, cold, nothrow, access, (assume)aligned?


static inline attribute(malloc, warn_unused_result, leaf, hot, nothrow, assume_aligned(8))
node_t *node_alloc(uint64_t chave) {
    void *ptr;
    int rv = posix_memalign(&ptr, sizeof(node_t *), sizeof(node_t));
    if (rv != 0) return NULL;

    node_t *no = ptr;
    no->color = RED;
    no->chave = chave;
    no->esq = no->dir = NULL;
    return no;
}

static inline attribute(nonnull, leaf, cold, nothrow)
void node_free(node_t *no) {
    if (no->esq != NULL) node_free(no->esq);
    if (no->dir != NULL) node_free(no->dir);
    free(no);
}

struct rbtree rb_empty_tree(void) {
    return (struct rbtree) {.head = NULL};
}

void rb_dealloc(struct rbtree *tree) {
    if (tree->head != NULL) {
        node_free(tree->head);
        tree->head = NULL;
    }
}
