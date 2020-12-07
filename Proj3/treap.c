#include "treap.h"
#include <stdlib.h>


typedef uint64_t prioroty_t;

typedef struct node node_t;

struct node {
    chave_t chave;
    prioroty_t pri;
    node_t *esq, *dir;
};


static inline
uint64_t random_pri(void) {
    uint32_t lo = random();
    uint32_t hi = random();

    uint64_t ans = hi;
    ans = (ans << 32) | lo;
    return ans;
}

static inline
node_t *node_alloc(chave_t chave) {
    node_t *no = malloc(sizeof(node_t));
    if unlikely(no == NULL) return NULL;

    no->chave = chave;
    no->pri = random_pri();
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
