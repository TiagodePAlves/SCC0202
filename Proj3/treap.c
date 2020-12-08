#include "treap.h"
#include "vec.h"
#include <stdlib.h>
/**
 *  Árvore Treap (Tree + Heap).
 *
 *  Os nós da árvore mantêm a propriedade de uma árvore binária de busca para
 * as chaves presentes, em que os nós esquerdos devem ter chaves menores que o
 * pai e os nós à direita devem ser maiores. Ao mesmo tempo, é mantida a
 * propriedade do heap de máximo para a prioridade, é que um nó tem sempre
 * prioridade maior que seus filhos.
 */


typedef struct node node_t;
/**
 * Estrutura de um nó.
 */
struct node {
    chave_t chave;
    priority_t pri;
    node_t *esq, *dir;
};


/* * * * * * * * * * *
 * (DE)INICIALIZAÇÃO *
 * * * * * * * * * * */

static inline attribute(returns_nonnull, hot, nothrow)
/**
 * Alocação de um novo nó.
 */
node_t *node_alloc(chave_t chave, priority_t prio) {
    node_t *no = malloc(sizeof(node_t));
    // erro de alocação
    if unlikely(no == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    };

    no->chave = chave;
    no->pri = prio;
    no->esq = no->dir = NULL;
    return no;
}

static inline attribute(nonnull, cold, nothrow)
/**
 * Desaloca o nó e seus filhos, recursivamente.
 */
void node_dealloc(node_t *no) {
    node_t *esq = no->esq;
    node_t *dir = no->dir;

    free(no);
    if (esq != NULL) node_dealloc(esq);
    if (dir != NULL) node_dealloc(dir);
}

// Nova árvore vazia.
struct treap treap_nova(void) {
    return (struct treap) {.raiz = NULL};
}

// Desaloca todos os nós da árvore.
void treap_dealloc(struct treap *arvore) {
    if (arvore->raiz != NULL) {
        node_dealloc(arvore->raiz);
        arvore->raiz = NULL;
    }
}

// BUSCA

static inline attribute(pure, nonnull, returns_nonnull, hot, nothrow)
node_t **busca_no(node_t **no_ptr, chave_t chave) {
    node_t *no = *no_ptr;
    while (no != NULL && no->chave != chave) {
        if (no->chave > chave) {
            no_ptr = &no->esq;
            no = no->esq;
        } else {
            no_ptr = &no->dir;
            no = no->dir;
        }
    }
    return no_ptr;
}

bool treap_busca(const struct treap *arvore, chave_t chave) {
    const node_t *no = *busca_no((node_t **) &arvore->raiz, chave);
    return no != NULL;
}

// INSERCAO

static inline attribute(nonnull, returns_nonnull, hot, nothrow)
node_t *rotaciona_esq(node_t *no) {
    node_t *dir = no->dir;
    no->dir = dir->esq;
    dir->esq = no;
    return dir;
}

static inline attribute(nonnull, returns_nonnull, hot, nothrow)
node_t *rotaciona_dir(node_t *no) {
    node_t *esq = no->esq;
    no->esq = esq->dir;
    esq->dir = no;
    return esq;
}

#define MIN_PRIO 0

static inline attribute(pure, hot, nothrow)
priority_t prioridade(const node_t *no) {
    return (no != NULL)? no->pri : MIN_PRIO;
}

static bool erro;

static inline attribute(hot, nothrow)
node_t *insere_chave(node_t *no, chave_t chave, priority_t prio) {
    if (no == NULL) {
        node_t *novo = node_alloc(chave, prio);
        erro = (novo == NULL);
        return novo;
    }

    if (no->chave > chave) {
        no->esq = insere_chave(no->esq, chave, prio);
        if (prioridade(no->esq) > no->pri) {
            no = rotaciona_dir(no);
        }
    } else if (no->chave < chave) {
        no->dir = insere_chave(no->dir, chave, prio);
        if (prioridade(no->dir) > no->pri) {
            no = rotaciona_esq(no);
        }
    } else {
        erro = true;
    }
    return no;
}

bool treap_insere(struct treap *arvore, chave_t chave, priority_t prio) {
    erro = false;
    arvore->raiz = insere_chave(arvore->raiz, chave, prio);
    return erro;
}

// REMOCAO

static inline attribute(nonnull, hot, nothrow)
void remove_no(node_t **no_ptr) {
    node_t *no = *no_ptr;

    while (no->esq != NULL || no->dir != NULL) {
        if (prioridade(no->esq) > prioridade(no->dir)) {
            *no_ptr = rotaciona_dir(no);
            no_ptr = &(*no_ptr)->dir;
        } else {
            *no_ptr = rotaciona_esq(no);
            no_ptr = &(*no_ptr)->esq;
        }
    }
    free(no);
    *no_ptr = NULL;
}

bool treap_remove(struct treap *arvore, chave_t chave) {
    node_t **no_ptr = busca_no(&arvore->raiz, chave);
    if unlikely(*no_ptr == NULL) return false;

    remove_no(no_ptr);
    return true;
}

// PERCURSO

typedef void callback_t(chave_t, priority_t);

static inline attribute(hot)
void percorre_ord_pos(const node_t *no, callback_t *op, bool ord) {
    if (no == NULL) return;

    percorre_ord_pos(no->esq, op, ord);
    if (ord) op(no->chave, no->pri);
    percorre_ord_pos(no->dir, op, ord);
    if (!ord) op(no->chave, no->pri);
}

#define INI_CAP 256ULL

static inline attribute(hot)
void percorre_prof_larg(const node_t *no, callback_t *op, bool prof) {
    if (no == NULL) return;

    struct vec vetor = vec_init(INI_CAP);
    do {
        op(no->chave, no->pri);
        node_t *pri = prof? no->dir : no->esq;
        node_t *seg = prof? no->esq : no->dir;

        if (pri != NULL) {
            vec_push_back(&vetor, (void *) pri);
        }
        if (seg != NULL) {
            vec_push_back(&vetor, (void *) seg);
        }
    } while ((no = prof? vec_pop_back(&vetor) : vec_pop_front(&vetor)) != NULL);

    vec_dealloc(&vetor);
}

void treap_percorre(const struct treap *arvore, void (*callback)(chave_t, priority_t), enum ordem ordem) {
    switch (ordem) {
        case PREORDEM:
            percorre_prof_larg(arvore->raiz, callback, true);
            break;
        case ORDEM:
            percorre_ord_pos(arvore->raiz, callback, true);
            break;
        case POSORDEM:
            percorre_ord_pos(arvore->raiz, callback, false);
            break;
        case LARGURA:
            percorre_prof_larg(arvore->raiz, callback, false);
            break;
        default:
            break;
    }
}
