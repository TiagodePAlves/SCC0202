#include "treap.h"
#include "vec.h"
#include <stdlib.h>



typedef struct node node_t;
struct node {
    chave_t chave;
    priority_t pri;
    node_t *esq, *dir;
};


// INIT

static inline attribute(returns_nonnull, hot, nothrow)
node_t *node_alloc(chave_t chave, priority_t prio) {
    node_t *no = malloc(sizeof(node_t));
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
void node_dealloc(node_t *no) {
    node_t *esq = no->esq;
    node_t *dir = no->dir;

    free(no);
    if (esq != NULL) node_dealloc(esq);
    if (dir != NULL) node_dealloc(dir);
}

struct treap treap_nova(void) {
    return (struct treap) {.raiz = NULL};
}

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
            no_ptr = &(*no_ptr)->esq;
        } else {
            *no_ptr = rotaciona_esq(no);
            no_ptr = &(*no_ptr)->dir;
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

#define INI_CAP 1024ULL

static inline attribute(nonnull, hot)
void percorre_pre_pos(const node_t *no, callback_t *op, bool pre) {
    struct vec vetor = vec_init(INI_CAP);
    vec_push_back(&vetor, no);

    while ((no = vec_pop_back(&vetor)) != NULL) {
        while ((pre? no->esq : no->dir) != NULL) {
            vec_push_back(&vetor, no);
            no = pre? no->esq : no->dir;
        }
        op(no->chave, no->pri);

        if ((pre? no->dir : no->esq) != NULL) {
            vec_push_back(&vetor, pre? no->dir : no->esq);
        }
    }
    vec_dealloc(&vetor);
}

static inline attribute(nonnull, hot)
void percorre_prof_larg(const node_t *no, callback_t *op, bool prof) {
    struct vec vetor = vec_init(INI_CAP);
    vec_push_back(&vetor, no);

    while ((no = prof? vec_pop_back(&vetor) : vec_pop_front(&vetor)) != NULL) {
        op(no->chave, no->pri);

        if (no->esq != NULL) {
            vec_push_back(&vetor, no->esq);
        }
        if (no->dir != NULL) {
            vec_push_back(&vetor, no->dir);
        }
    }
    vec_dealloc(&vetor);
}

void treap_percorre(const struct treap *arvore, void (*callback)(chave_t, priority_t), enum ordem ordem) {
    if (arvore->raiz == NULL) return;

    switch (ordem) {
        case PREORDEM:
            percorre_pre_pos(arvore->raiz, callback, true);
            break;
        case ORDEM:
            percorre_prof_larg(arvore->raiz, callback, true);
            break;
        case POSORDEM:
            percorre_pre_pos(arvore->raiz, callback, false);
            break;
        case LARGURA:
            percorre_prof_larg(arvore->raiz, callback, false);
            break;
        default:
            break;
    }
}
