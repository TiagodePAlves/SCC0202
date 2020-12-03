#include "rbtree.h"
#include <stdlib.h>

/**
 * Propriedades: // TODO
 *  3 - Nenhum caminho partindo da raiz contém dois nós vermelhos.
 *  4 - Partindo de um nó qualquer, todos os caminhos até
 *      as folhas contêm o mesmo número de nós pretos.
 */

typedef struct node attribute(aligned(4)) node_t;

typedef enum color {
    NEGRA = 0,
    RUBRO
} color_t;

typedef uint32_t chave_t;

struct node {
    color_t cor;
    chave_t chave;
    node_t *esq, *dir;
} attribute(aligned(4));

_Static_assert(sizeof(color_t) == 4, "");
_Static_assert(sizeof(node_t) == 24, "");
_Static_assert(sizeof(node_t *) == 8, "");

// TODO: leaf, hot, cold, nothrow, access, (assume)aligned?

// INIT

static inline attribute(nonnull, leaf, cold, nothrow)
void no_free(node_t *no) {
    if (no->esq != NULL) no_free(no->esq);
    if (no->dir != NULL) no_free(no->dir);
    free(no);
}

struct rbtree rb_nova(void) {
    return (struct rbtree) {.raiz = NULL};
}

void rb_dealloc(struct rbtree *tree) {
    if (tree->raiz != NULL) {
        no_free(tree->raiz);
        tree->raiz = NULL;
    }
}

// NO OPS


