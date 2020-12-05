#include "rbtree.h"
#include <stdlib.h>
/**
 *  Implementação da Árvore Rubro-Negra (Left Leaning).
 *
 * Nessa árvore, um nó pode ser marcado como vermelho para indicar que
 * esse nó e o seu pai podem ser vistos como um único nó com três filhos.
 * Observando a árvore dessa forma, podemos ver que as operações
 * adicionais servem para transformar nós simples (2 filhos pretos) em
 * nos-3 (1 filho vermelho), buscando manter uma mesma altura em
 * todas as subárvores.
 *
 * Como a árvore é de fato implementada como uma árvore binária, a
 * altura que deve ser mantida é considerando apenas os nós pretos.
 * Além disso, para que um nó seja no máximo nó-3, não podem existir
 * dois nós vermelhos em sequência e um nó preto também não pode ter
 * dois filhos vermelhos. Por fim, como a raiz não tem pai, não faz
 * sentido ela ser vemelha.
 *
 * Ficamos então com as seguintes propriedades da árvore:
 *
 *  1 - A raiz é preta.
 *  2 - Partindo de um nó qualquer, todos os caminhos até as folhas
 *      contêm o mesmo número de nós pretos.
 *  3 - Nenhum caminho partindo contém dois nós vermelhos em sequência.
 *  4 - Todo nó vermelho é o filho esquerdo de seu pai.
 *  5 - É uma árvore binária de busca.
 *  6 - Uma folha (NULL) é considerada preta.
 */

// Cor de um nó
typedef enum color {
    NEGRA = 0,
    RUBRO
} color_t;

typedef struct node node_t;
/**
 * Estrutura de um nó, com a cor, a chave e os filhos.
 */
struct node {
    color_t cor;
    chave_t chave;
    node_t *esq, *dir;
} attribute(aligned(4));


/* * * * * * * * * * *
 * (DE)INICIALIZAÇÃO *
 * * * * * * * * * * */

static inline attribute(malloc, warn_unused_result, nothrow)
/**
 * Alocação de um novo nó vermelho.
 */
node_t *no_alloc(chave_t chave) {
    node_t *no = malloc(sizeof(node_t));
    // erro de alocação
    if unlikely(no == NULL) return NULL;

    no->cor = RUBRO;
    no->chave = chave;
    no->esq = no->dir = NULL;
    return no;
}

static inline attribute(nonnull, cold, nothrow)
/**
 * Desalocação de um nó e de seu filhos.
 */
void no_free(node_t *no) {
    if (no->esq != NULL) no_free(no->esq);
    if (no->dir != NULL) no_free(no->dir);
    free(no);
}

// Nova árvore vazia.
struct rbtree rb_nova(void) {
    return (struct rbtree) {.raiz = NULL};
}

// Desalocação da árvore e de seu nós.
void rb_dealloc(struct rbtree *tree) {
    if (tree->raiz != NULL) {
        no_free(tree->raiz);
        // marca como vazia novamente
        tree->raiz = NULL;
    }
}


/* * * * * * * * * * *
 * OPERAÇÕES DA LLRB *
 * * * * * * * * * * */

static inline attribute(hot, nothrow)
/**
 * Checa se um nó é vermelho.
 */
bool vermelho(const node_t *no) {
    // considera folhas (NULL) como pretos
    return (no != NULL) && (no->cor != NEGRA);
}

static inline attribute(nonnull, returns_nonnull, hot, nothrow)
/**
 *  Rotação de um nó-3 com filho direito vermelho.
 *
 * A rotação mantém a representação do par como um conjunto com três
 * filhos, mudando apenas o seu posicionamento na árvore binária.
 * Note que o nó original quebra a propriedade 4.
 *
 * Essa operação mantém todas as propriedades válidas na subárvore.
 */
node_t *rotaciona_esq(node_t *no) {
    // filho direito deve ser válido e vermelho
    node_t *dir = no->dir;

    no->dir = dir->esq;
    dir->esq = no;
    dir->cor = no->cor;
    no->cor = RUBRO;

    return dir;
}

static inline attribute(nonnull, returns_nonnull, hot, nothrow)
/**
 * Rotação de um nó-3 com filho esquerdo vermelho.
 *
 * Quebra a propriedade 4.
 */
node_t *rotaciona_dir(node_t *no) {
    // filho esquerdo deve ser válido e vermelho
    node_t *esq = no->esq;

    no->esq = esq->dir;
    esq->dir = no;
    esq->cor = no->cor;
    no->cor = RUBRO;

    return esq;
}

static inline attribute(nonnull, hot, nothrow)
/**
 * Inversão das cores de um nó e de seus dois filhos.
 * Usada para garantir a propriedade 4.
 *
 * Pode quebrar as propriedades 1 e 3.
 */
void inverte_cores(node_t *no) {
    no->cor = !no->cor;
    no->dir->cor = !no->dir->cor;
    no->esq->cor = !no->esq->cor;
}


/* * * * * * *
 * INSERÇÃO  *
 * * * * * * */

static inline attribute(nonnull, hot, nothrow)
/**
 * Modifica nós recursivamente para inserir um novo nó.
 * Mantém as propriedades 2, 3, 4 e 5 na sua subárvore.
 *
 * Retorna se ocorreu erro de alocação. Nesse caso, a árvore não é alterada.
 */
bool insere_no(node_t **no_ptr, chave_t chave) {
    node_t *no = *no_ptr;
    // no vazio, insere novo
    if (no == NULL) {
        node_t *novo = no_alloc(chave);
        // erro de alocação
        if unlikely(novo == NULL) return true;

        *no_ptr = novo;
        return false;
    }

    bool erro;
    // insere em uma das subárvores, mantendo 5
    if (chave < no->chave) {
        erro = insere_no(&no->esq, chave);
    } else {
        erro = insere_no(&no->dir, chave);
    }
    if unlikely(erro) return true;

    // checa propriedade 4
    if (vermelho(no->dir) && !vermelho(no->esq)) {
        no = rotaciona_esq(no);
    }
    // checa propriedade 3
    if (vermelho(no->esq) && vermelho(no->esq->esq)) {
        // quebra a prop. 4 para corrigir abaixo
        no = rotaciona_dir(no);
    }
    // garante a prop. 4, mantendo a prop. 2
    if (vermelho(no->esq) && vermelho(no->dir)) {
        inverte_cores(no);
    }

    *no_ptr = no;
    return false;
}

// Inserção na árvore.
bool rb_insere(struct rbtree *arvore, chave_t chave) {
    bool erro = insere_no(&arvore->raiz, chave);
    // erro de alocação
    if unlikely(erro) return true;

    // garante propriedade 1
    arvore->raiz->cor = NEGRA;
    return false;
}


/* * * * *
 * BUSCA *
 * * * * */

static inline attribute(pure, hot, nothrow)
/**
 * Busca nó com menor chave na subárvore.
 *
 * Retorna NULL se não existir (árvore vazia).
 */
const node_t *busca_min(const node_t *no) {
    if (no == NULL) return NULL;
    while (no->esq != NULL) {
        no = no->esq;
    }
    return no;
}

static inline attribute(pure, hot, nothrow)
/**
 * Busca nó com maior chave na subárvore.
 *
 * Retorna NULL se não existir (árvore vazia).
 */
const node_t *busca_max(const node_t *no) {
    if (no == NULL) return NULL;
    while (no->dir != NULL) {
        no = no->dir;
    }
    return no;
}

static inline attribute(pure, hot, nothrow)
/**
 * Busca sucessor (`succ == true`) ou predecessor (`succ == false`) de
 * uma chave na árvore, considerando percurso em-ordem.
 *
 * Retorna NULL se a chave não existir na árvore ou se ela não tiver
 * successor ou predecessor.
 */
const node_t *no_succ_pred(const node_t *no, chave_t chave, bool succ) {
    // nó mais próximo da raiz que é algum sucessor ou predecessor da chave
    // se ela existir
    const node_t *ant = NULL;
    // busca do nó
    while (no != NULL && no->chave != chave) {
        if (no->chave < chave) {
            // possível predecessor da chave
            ant = succ? ant : no;
            no = no->dir;
        } else {
            // possível sucessor
            ant = succ? no : ant;
            no = no->esq;
        }
    }
    // nó não encontrado
    if (no == NULL) return NULL;

    const node_t *res = succ?
        // sucessor será o menor nó na subaŕvore direita
        busca_min(no->dir) :
        // predecessor será o maior na esquerda
        busca_max(no->esq);

    if (res != NULL) {
        return res;
    // não existe sucessor ou predecessor entre os filhos
    } else {
        // retorna o suc./pred. entre os pais
        return ant;
    }
}

// Busca do succesor.
bool rb_busca_succ(const struct rbtree *arvore, chave_t chave, chave_t *succ) {
    const node_t *no = no_succ_pred(arvore->raiz, chave, true);
    // sucessor não encontrado
    if (no == NULL) return true;

    *succ = no->chave;
    return false;
}

// Busca do predecessor.
bool rb_busca_pred(const struct rbtree *arvore, chave_t chave, chave_t *pred) {
    const node_t *no = no_succ_pred(arvore->raiz, chave, false);
    // predecessor não encontrado
    if (no == NULL) return true;

    *pred = no->chave;
    return false;
}

// Busca do máximo.
bool rb_busca_max(const struct rbtree *arvore, chave_t *max) {
    const node_t *no = busca_max(arvore->raiz);
    // árvore vazia
    if unlikely(no == NULL) return true;

    *max = no->chave;
    return false;
}

// Busca do mínimo.
bool rb_busca_min(const struct rbtree *arvore, chave_t *min) {
    const node_t *no = busca_min(arvore->raiz);
    // árvore vazia
    if unlikely(no == NULL) return true;

    *min = no->chave;
    return false;
}


/* * * * * * * * *
 * PERCORRIMENTO *
 * * * * * * * * */

// Tipos de percurso.
typedef enum ordem {
    PRE_ORDEM, EM_ORDEM, POS_ORDEM
} ordem_t;

static inline attribute(hot)
/**
 * Percorre a árvore em percurso `ordem` aplicando a função em cada chave.
 */
void percorre(const node_t *no, void (*callback)(chave_t chave), ordem_t ordem) {
    // folhas não têm chaves
    if (no == NULL) return;

    // pré-ordem: pai impresso primeiro
    if (ordem == PRE_ORDEM) callback(no->chave);
    percorre(no->esq, callback, ordem);
    // em-ordem: pai impresso após esquerdo
    if (ordem == EM_ORDEM) callback(no->chave);
    percorre(no->dir, callback, ordem);
    // pós-ordem: pai impresso após filhos
    if (ordem == POS_ORDEM) callback(no->chave);
}

// Percurso pré-ordem.
void rb_pre_ordem(const struct rbtree *arvore, void (*callback)(chave_t chave)) {
    percorre(arvore->raiz, callback, PRE_ORDEM);
}

// Percurso em-ordem.
void rb_em_ordem(const struct rbtree *arvore, void (*callback)(chave_t chave)) {
    percorre(arvore->raiz, callback, EM_ORDEM);
}

// Percurso pós-ordem.s
void rb_pos_ordem(const struct rbtree *arvore, void (*callback)(chave_t chave)) {
    percorre(arvore->raiz, callback, POS_ORDEM);
}

