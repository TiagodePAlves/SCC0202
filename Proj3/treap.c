#include "treap.h"
#include <stdlib.h>


typedef uint64_t priority_t;

typedef struct node node_t;

struct node {
    chave_t chave;
    priority_t pri;
    node_t *esq, *dir;
};


// INIT

static inline
/**
 * Quantidade de ticks desde que a máquina foi ligada.
 *
 * Serve como um marcador de "tempo" sem a <time.h>,
 * mas é dependente da arquitetura.
 */
uint64_t ticks(void) {
    // ReaD TimeStamp Counter
    asm("rdtsc");
    // o resultado da instrução acima é
    // guardado nos registradores EAX e EDX
    register uint32_t lo asm("eax");
    register uint32_t hi asm("edx");

    // que devem então ser combinados
    // em um inteiro de 64 bits
    uint64_t low = (uint64_t) lo;
    uint64_t high = (uint64_t) hi;
    return (high << 32) | low;
}

static inline
priority_t random_pri(void) {
    static bool init = false;
    if unlikely(!init) {
        srandom(ricks());
        init = true;
    }

    const long MASK = (1 << 31) - 1;
    uint32_t lo = random() & MASK;
    uint32_t hi = random();

    uint64_t ans = hi;
    ans = (ans << 32) | (lo + 1);
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

// BUSCA

static inline
bool node_busca(const node_t *no, chave_t chave) {
    if (no == NULL) {
        return false;
    } else if (no->chave == chave) {
        return true;
    } else if (no->chave > chave) {
        return node_busca(no->esq, chave);
    } else {
        return node_busca(no->dir, chave);
    }
}

bool treap_busca(const struct treap *arvore, chave_t chave) {
    return node_busca(arvore->raiz, chave);
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
node_t *insere_chave(node_t *no, chave_t chave) {
    if (no == NULL) {
        node_t *novo = node_alloc(chave);
        erro = (novo == NULL);
        return novo;
    }

    if (no->chave > chave) {
        no->esq = insere_chave(no->esq, chave);
        if (prioridade(no->esq) > no->pri) {
            no = rotaciona_dir(no);
        }
    } else if (no->chave < chave) {
        no->dir = insere_chave(no->dir, chave);
        if (prioridade(no->dir) > no->pri) {
            no = rotaciona_esq(no);
        }
    } else {
        erro = true;
    }
    return no;
}

int treap_insere(struct treap *arvore, chave_t chave) {
    erro = false;
    arvore->raiz = insere_chave(arvore->raiz, chave);
    return erro? -1 : 0;
}
