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

static inline attribute(malloc, warn_unused_result, leaf, nothrow)
node_t *no_alloc(chave_t chave) {
    node_t *no = malloc(sizeof(node_t));
    if unlikely(no == NULL) return NULL;

    no->cor = RUBRO;
    no->chave = chave;
    no->esq = no->dir = NULL;
    return no;
}

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

static inline attribute(nonnull, returns_nonnull, leaf, hot, nothrow, access(read_write, 1))
node_t *rotaciona_esq(node_t *no) {
    node_t *dir = no->dir;

    no->dir = dir->esq;
    dir->esq = no;
    dir->cor = no->cor;
    no->cor = RUBRO;

    return dir;
}

static inline attribute(nonnull, returns_nonnull, leaf, hot, nothrow, access(read_write, 1))
node_t *rotaciona_dir(node_t *no) {
    node_t *esq = no->esq;

    no->esq = esq->dir;
    esq->dir = no;
    esq->cor = no->cor;
    no->cor = RUBRO;

    return esq;
}

static inline attribute(nonnull, returns_nonnull, leaf, hot, nothrow, access(read_write, 1))
void inverte_cores(node_t *no) {
    no->cor = !no->cor;
    no->dir->cor = !no->dir->cor;
    no->esq->cor = !no->esq->cor;
}

// INSERT

static bool erro;

static inline attribute(leaf, hot, nothrow, access(read_only, 1))
bool vermelho(const node_t *no) {
    return (no != NULL) && (no->cor != NEGRA);
}

static inline attribute(leaf, hot, nothrow, access(read_write, 1))
node_t *insere_no(node_t *no, chave_t chave) {
    if (no == NULL) {
        node_t *novo = no_alloc(chave);
        erro = (novo == NULL);
        return novo;
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


// BUSCA

static inline attribute(pure, leaf, hot, nothrow, access(read_only, 1))
const node_t *busca_no(const node_t *raiz, chave_t chave) {
    for (node_t *no = raiz; no != NULL;) {
        if (no->chave < chave) {
            no = no->dir;
        } else if (no->chave > chave) {
            no = no->esq;
        } else {
            return no;
        }
    }
    return NULL;
}

static inline attribute(pure, leaf, hot, nothrow, access(read_only, 1))
const node_t *busca_min(const node_t *no) {
    if (no == NULL) return NULL;
    while (no->esq != NULL) {
        no = no->esq;
    }
    return no;
}

static inline attribute(pure, leaf, hot, nothrow, access(read_only, 1))
const node_t *busca_max(const node_t *no) {
    if (no == NULL) return NULL;
    while (no->dir != NULL) {
        no = no->dir;
    }
    return no;
}

bool rb_busca_succ(const struct rbtree *arvore, chave_t chave, chave_t *succ) {
    const node_t *no = busca_no(arvore->raiz, chave);
    if (no == NULL) return true;

    const node_t *min = busca_min(no->dir);
    if (min == NULL) return true;

    *succ = min->chave;
    return false;
}

bool rb_busca_pred(const struct rbtree *arvore, chave_t chave, chave_t *pred) {
    const node_t *no = busca_no(arvore->raiz, chave);
    if (no == NULL) return true;

    const node_t *max = busca_max(no->esq);
    if (max == NULL) return true;

    *pred = max->chave;
    return false;
}

bool rb_busca_max(const struct rbtree *arvore, chave_t *max) {
    const node_t *no = busca_max(arvore->raiz);
    if (no == NULL) return false;

    *max = no->chave;
    return true;
}

bool rb_busca_min(const struct rbtree *arvore, chave_t *min) {
    const node_t *no = busca_min(arvore->raiz);
    if (no == NULL) return false;

    *min = no->chave;
    return true;
}

// PREORDEM? HASKELL FOLD? PERCORRE?

typedef void callback_t(chave_t chave, void *data);

typedef enum ordem {
    PRE_ORDEM, EM_ORDEM, POS_ORDEM
} ordem_t;

static inline attribute(hot, access(read_only, 1))
void percorre(const node_t *no, callback_t *callback, void *data, ordem_t ordem) {
    if (no == NULL) return;

    if (ordem == PRE_ORDEM) callback(no->chave, data);
    percorre(no->esq, callback, data, ordem);
    if (ordem == EM_ORDEM) callback(no->chave, data);
    percorre(no->dir, callback, data, ordem);
    if (ordem == POS_ORDEM) callback(no->chave, data);
}

void rb_pre_ordem(const struct rbtree *arvore, void (*callback)(chave_t chave, void *data), void *data) {
    percorre(arvore->raiz, callback, data, PRE_ORDEM);
}

void rb_em_ordem(const struct rbtree *arvore, void (*callback)(chave_t chave, void *data), void *data) {
    percorre(arvore->raiz, callback, data, EM_ORDEM);
}

void rb_pos_ordem(const struct rbtree *arvore, void (*callback)(chave_t chave, void *data), void *data) {
    percorre(arvore->raiz, callback, data, POS_ORDEM);
}

