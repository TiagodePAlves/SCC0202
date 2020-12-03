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

node_t *rotaciona_esq(node_t *no) {
    node_t *dir = no->dir;

    no->dir = dir->esq;
    dir->esq = no;
    dir->cor = no->cor;
    no->cor = RUBRO;

    return dir;
}

node_t *rotaciona_dir(node_t *no) {
    node_t *esq = no->esq;

    no->esq = esq->dir;
    esq->dir = no;
    esq->cor = no->cor;
    no->cor = RUBRO;

    return esq;
}

void inverte_cores(node_t *no) {
    no->cor = !no->cor;
    no->dir->cor = !no->dir->cor;
    no->esq->cor = !no->esq->cor;
}

// INSERT

static bool erro;

static inline attribute(malloc, warn_unused_result, leaf, nothrow)
node_t *no_alloc(chave_t chave) {
    node_t *no = malloc(sizeof(node_t));
    if unlikely(no == NULL) {
        erro = true;
        return NULL;
    }

    no->cor = RUBRO;
    no->chave = chave;
    no->esq = no->dir = NULL;
    return no;
}

bool vermelho(node_t *no) {
    return (no != NULL) && (no->cor != NEGRA);
}

node_t *insere_no(node_t *no, chave_t chave) {
    if (no == NULL) {
        return no_alloc(chave);
    }

    if (no->chave < chave) {
        no->esq = insere_no(no->esq, chave);
    } else {
        no->dir = insere_no(no->dir, chave);
    }

    if (vermelho(no->dir) && !vermelho(no->esq)) {
        no = rotaciona_esq(no);
    }
    if (vermelho(no->esq) && vermelho(no->esq->esq)) {
        no = rotaciona_dir(no);
    }
    if (vermelho(no->esq) && vermelho(no->dir)) {
        inverte_cores(no);
    }
    return no;
}

bool rb_insere(struct rbtree *arvore, chave_t chave) {
    erro = false;
    arvore->raiz = insere_no(arvore->raiz, chave);
    return erro;
}


