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


/* * * * * * *
 * INSERÇÃO  *
 * * * * * * */

static inline attribute(nonnull, returns_nonnull, hot, nothrow)
/**
 * Rotação à esquerda.
 *
 * Mantém a propriedade de ABB, mas pode alterar o heap.
 */
node_t *rotaciona_esq(node_t *no) {
    node_t *dir = no->dir;
    no->dir = dir->esq;
    dir->esq = no;
    return dir;
}

static inline attribute(nonnull, returns_nonnull, hot, nothrow)
/**
 * Rotação à direita.
 *
 * Mantém a propriedade de ABB, mas pode alterar o heap.
 */
node_t *rotaciona_dir(node_t *no) {
    node_t *esq = no->esq;
    no->esq = esq->dir;
    esq->dir = no;
    return esq;
}

// Prioridade mínima possível
#define MIN_PRIO 0

static inline attribute(pure, hot, nothrow)
/**
 * Prioridade de um nó ou folha.
 */
priority_t prioridade(const node_t *no) {
    // folhas tem prioridade mínima
    return (no != NULL)? no->pri : MIN_PRIO;
}

// Marcador de chave já existente.
static bool existe;

static inline attribute(hot, nothrow)
/**
 * Insere novo nó na subárvore com raiz em nó e retorna nova raiz.
 *
 * Mantém a proriedade de ABB em toda a árvore e a de heap pelo menos na
 * subárvore.
 */
node_t *insere_chave(node_t *no, chave_t chave, priority_t prio) {
    // insere nó no lugar da folha
    if (no == NULL) {
        return node_alloc(chave, prio);
    }

    // inserção mantendo ABB
    if (no->chave > chave) {
        no->esq = insere_chave(no->esq, chave, prio);
        // rotação para manter o heap
        if (prioridade(no->esq) > no->pri) {
            no = rotaciona_dir(no);
        }
    } else if (no->chave < chave) {
        no->dir = insere_chave(no->dir, chave, prio);
        // rotação para manter o heap
        if (prioridade(no->dir) > no->pri) {
            no = rotaciona_esq(no);
        }
    } else {
        // chave encontrada na árvore
        existe = true;
    }
    return no;
}

// Inserção na árvore.
bool treap_insere(struct treap *arvore, chave_t chave, priority_t prio) {
    // marca como inexistente
    existe = false;
    // insere pela raiz
    arvore->raiz = insere_chave(arvore->raiz, chave, prio);
    // retorna o marcador, que pode ter sido alterado
    return existe;
}


/* * * * *
 * BUSCA *
 * * * * */

static inline attribute(pure, nonnull, returns_nonnull, hot, nothrow)
/**
 * Busca de um nó considerando a propriedade de ABB.
 *
 * Recebe um ponteiro onde a raiz da árvore está alocada e retorna um ponteiro
 * onde um nó com aquela chave aparece ou deveria aparecer.
 */
node_t **busca_no(node_t **no_ptr, chave_t chave) {
    node_t *no = *no_ptr;
    // até achar uma folha ou um nó com a chave
    while (no != NULL && no->chave != chave) {
        if (no->chave > chave) {
            // endereço do próximo nó
            no_ptr = &no->esq;
            // próximo nó
            no = no->esq;
        } else {
            // endereço do próximo nó
            no_ptr = &no->dir;
            // próximo nó
            no = no->dir;
        }
    }
    return no_ptr;
}

// Busca por chave
bool treap_busca(const struct treap *arvore, chave_t chave) {
    // cast sem const, mas a árvore ainda não será alterada
    const node_t *no = *busca_no((node_t **) &arvore->raiz, chave);
    // teste se não tem nenhum nó com aquela chave
    return no != NULL;
}


/* * * * * *
 * REMOÇÃO *
 * * * * * */

static inline attribute(nonnull, hot, nothrow, unused)
/**
 * Remove o nó na raiz da subárvore, mantendo as propriedades.
 *
 * Recebe um ponteiro onde a raiz da árvore está alocada. A nova raiz será
 * escrita nele novamente.
 */
void remove_no(node_t **no_ptr) {
    node_t *no = *no_ptr;
    // até o nó ser uma folha
    while (no->esq != NULL || no->dir != NULL) {
        // rotaciona para que o filho com maior prioridade
        // seja a nova raiz
        if (prioridade(no->esq) > prioridade(no->dir)) {
            // muda a raiz
            *no_ptr = rotaciona_dir(no);
            // novo local do nó
            no_ptr = &(*no_ptr)->dir;
        } else {
            // muda a raiz
            *no_ptr = rotaciona_esq(no);
            // novo local do nó
            no_ptr = &(*no_ptr)->esq;
        }
    }
    // libera o nó e repõe a folha
    free(no);
    *no_ptr = NULL;
}

static inline attribute(nonnull, hot, nothrow, unused)
/**
 * Remove o nó na raiz da subárvore, rotacionando apenas para a esquerda.
 *
 * PODE QUEBRAR A PROPRIEDADE DO HEAP.
 */
void remove_no_rotesq(node_t **no_ptr) {
    node_t *no = *no_ptr;
    // até o nó não ter filho direito
    while (no->dir != NULL) {
        // muda a raiz
        *no_ptr = rotaciona_esq(no);
        // novo local do nó
        no_ptr = &(*no_ptr)->esq;
    }
    // coloca o filho esquerdo no lugar
    *no_ptr = no->esq;
    // e remove o nó
    free(no);
}

// Remoção da chave
bool treap_remove(struct treap *arvore, chave_t chave) {
    // busca nó com chave
    node_t **no_ptr = busca_no(&arvore->raiz, chave);
    if unlikely(*no_ptr == NULL) return false;
    // e remove ele da sua subárvore
    remove_no_rotesq(no_ptr);
    return true;
}


/* * * * * * * * *
 * PERCORRIMENTO *
 * * * * * * * * */

// Tipo para as funções chamadas no percorrimento.
typedef void callback_t(chave_t, priority_t);

static inline attribute(hot)
/**
 * Percorre em ordem (`ord == true`) ou pós ordem (`ord == false`).
 *
 * Implementado de forma recursiva.
 */
void percorre_ord_pos(const node_t *no, callback_t *op, bool ord) {
    if (no == NULL) return;

    percorre_ord_pos(no->esq, op, ord);
    // em ordem: pai depois do filho esquerdo
    if (ord) op(no->chave, no->pri);
    percorre_ord_pos(no->dir, op, ord);
    // pós ordem: pai depois dos filhos
    if (!ord) op(no->chave, no->pri);
}

// Capacidade incial da pilha/fila
#define INI_CAP 256ULL

static inline attribute(hot)
/**
 * Percorre em pré ordem (`prof == true`) ou largura (`prof == false`).
 *
 * Implementado com uma pilha ou uma fila.
 */
void percorre_prof_larg(const node_t *no, callback_t *op, bool prof) {
    if (no == NULL) return;

    // vetor com os nós, usado como pilha (pré ordem) ou fila (largura)
    struct vec vetor = vec_init(INI_CAP);
    do {
        op(no->chave, no->pri);
        // pŕoximo o nó a ser inserido na pilha/fila
        node_t *pri = prof? no->dir : no->esq;
        if (pri != NULL) {
            vec_push_back(&vetor, (void *) pri);
        }
        // nó seguinte (segundo nó) a ser inserido
        node_t *seg = prof? no->esq : no->dir;
        if (seg != NULL) {
            vec_push_back(&vetor, (void *) seg);
        }
    // próximo nó da pilha ou fila, até acabar o vetor
    } while ((no = prof? vec_pop_back(&vetor) : vec_pop_front(&vetor)) != NULL);

    vec_dealloc(&vetor);
}

// Percorre
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
