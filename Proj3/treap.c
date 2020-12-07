#include "treap.h"
#include <stdlib.h>



typedef struct node node_t;
struct node {
    chave_t chave;
    priority_t pri;
    node_t *esq, *dir;
};


// INIT

static inline attribute(malloc)
node_t *node_alloc(chave_t chave, priority_t prio) {
    node_t *no = malloc(sizeof(node_t));
    if unlikely(no == NULL) return NULL;

    no->chave = chave;
    no->pri = prio;
    no->esq = no->dir = NULL;
    return no;
}

static inline
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

static inline
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

static inline
node_t *rotaciona_esq(node_t *no) {
    node_t *dir = no->dir;
    no->dir = dir->esq;
    dir->esq = no;
    return dir;
}

static inline
node_t *rotaciona_dir(node_t *no) {
    node_t *esq = no->esq;
    no->esq = esq->dir;
    esq->dir = no;
    return esq;
}

#define MIN_PRIO 0

static inline
priority_t prioridade(const node_t *no) {
    return (no != NULL)? no->pri : MIN_PRIO;
}

static bool erro;

static inline
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

int treap_insere(struct treap *arvore, chave_t chave, priority_t prio) {
    erro = false;
    arvore->raiz = insere_chave(arvore->raiz, chave, prio);
    return erro? -1 : 0;
}

// REMOCAO

static inline
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

int treap_remove(struct treap *arvore, chave_t chave) {
    node_t **no_ptr = busca_no(&arvore->raiz, chave);
    if unlikely(*no_ptr == NULL) return -1;

    remove_no(no_ptr);
    return 0;
}
