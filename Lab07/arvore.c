#include "arvore.h"
#include <stdlib.h>
#include <stdio.h>


/* Nó de uma árvore binária. */
typedef struct no {
    // Id do nó.
    unsigned id;
    // Filho esquerdo.
    struct no *esq;
    // Filho direito.
    struct no *dir;
} no_t;

// Árvore binária
struct arvore {
    // Raiz da árvore.
    no_t *raiz;
};


/* * * * * * * * * * * * * *
 * ALOCAÇÃO E DESALOCAÇÃO  */

static
/**
 * Aloca novo nó com o dado id.
 *
 * Encerra o programa em erro de alocação.
 */
no_t *no_novo(unsigned id) {
    no_t *novo = malloc(sizeof(no_t));
    // não alocado
    if (novo == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    // monta o nó como folha
    novo->id = id;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

static
/**
 * Aloca nova árvore vazia.
 *
 * Encerra o programa em erro de alocação.
 */
arvore_t *arvore_nova(void) {
    arvore_t *nova = malloc(sizeof(arvore_t));
    // erro de alocação
    if (nova == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    // monta árvore vazia
    nova->raiz = NULL;
    return nova;
}

static
/**
 * Desaloca os filhos do nó e depois o próprio nó
 * de forma recursiva. Ordem pósfixa.
 */
void no_destroi(no_t *no) {
    // nada a desalocar
    if (no == NULL) {
        return;
    }

    no_destroi(no->esq);
    no_destroi(no->dir);
    free(no);
}

/* Desaloca árvore inteira. */
void arvore_destroi(arvore_t *arvore) {
    no_destroi(arvore->raiz);
    free(arvore);
}


/* * * * * * * * * * * * * *
 * CONSTRUÇÃO PELA ENTRADA */

static
/**
 * Monta um nó da árvore de forma recursiva, usando
 * o vetor de entradas.
 */
no_t *monta_no(const entrada_t *entrada, size_t len, int id) {
    // id inválido
    if (id < 0 || id >= len) {
        // é um filho vazio
        return NULL;
    }

    // monta o nó com o id
    no_t *no = no_novo(id);
    // monta os filhos recursivamente
    no->esq = monta_no(entrada, len, entrada[id].e);
    no->dir = monta_no(entrada, len, entrada[id].d);

    return no;
}

/* Monta uma árvore a partir do vetor de entradas. */
arvore_t *monta_arvore(const entrada_t entrada[], size_t len) {
    // aloca a árvore
    arvore_t *arv = arvore_nova();
    // inici recursão pela raiz (id 0)
    arv->raiz = monta_no(entrada, len, 0);
    return arv;
}


/* * * * * * * * *
 * MEDIDAS DO NÓ */

static
/**
 * Cálculo da altura de um nó de forma recursiva.
 */
size_t altura_no(const no_t *no) {
    // nó vazio tem altura 0
    if (no == NULL) {
        return 0;
    }
    // outros nós são um nível mais alto
    // que o maior de seus filhos
    size_t hdir = altura_no(no->dir);
    size_t hesq = altura_no(no->esq);

    if (hesq > hdir) {
        return hesq + 1;
    } else {
        return hdir + 1;
    }
}

static
/**
 * Grau de um nó não vazio. Em ABs, é 0, 1 ou 2.
 */
unsigned grau_no(const no_t *no) {
    // influencia do filho esquerdo no grau do nó
    unsigned esq = (no->esq != NULL);
    // influencia do direito
    unsigned dir = (no->dir != NULL);
    // grau fica como a soma das duas
    return esq + dir;
}

static
/**
 * Id do nó. Se o nó não existe, o id é VAZIO.
 */
int no_id(const no_t *no) {
    if (no != NULL) {
        return no->id;
    } else {
        return VAZIO;
    }
}

static
/**
 * Id dos filhos de um nó.
 */
entrada_t filhos_no(const no_t *no) {
    // monta uma 'entrada_t' com os resultados
    return (entrada_t) {
        .e = no_id(no->esq),
        .d = no_id(no->dir)
    };
}

static
/**
 * String estática descrevendo o tipo de um nó.
 */
const char *tipo_no(const no_t *no, const no_t *pai) {
    // raiz não tem pai
    if (pai == NULL) {
        return "raiz";
    // folha não tem filhos
    } else if (grau_no(no) == 0) {
        return "folha";
    // o resto é interno
    } else {
        return "interno";
    }
}


/* * * * * * * * * * *
 * IMPRESSÃO DOS NÓS */

static
/**
 * Imprissão dos nós recursivamente, em ordem préfixa.
 */
void imprime_no(FILE *stream, const no_t *no, const no_t *pai) {
    // nó vazio
    if (no == NULL) {
        return;
    }

    // calcula as medidas do nó
    unsigned id = no->id;
    int id_pai = no_id(pai);
    size_t altura = altura_no(no);
    unsigned grau = grau_no(no);
    entrada_t filho = filhos_no(no);
    const char *tipo = tipo_no(no, pai);
    // e imprime elas
    (void) fprintf(stream,
        "no %u: pai = %d, altura = %zu, grau = %u, filhos = (%d,%d), tipo = %s\n",
        id, id_pai, altura, grau, filho.e, filho.d, tipo
    );

    // depois imprime os filhos
    imprime_no(stream, no->esq, no);
    imprime_no(stream, no->dir, no);
}

/* Impressão da árvore completa. */
void imprime_arvore_prefixa(FILE *stream, const arvore_t *arvore) {
    imprime_no(stream, arvore->raiz, NULL);
}
