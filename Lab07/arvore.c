#include "arvore.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct no {
    unsigned id;
    struct no *esq;
    struct no *dir;
} no_t;

struct arvore {
    no_t *raiz;
};


static
no_t *no_novo(unsigned id) {
    no_t *novo = malloc(sizeof(no_t));
    if (novo == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    novo->id = id;
    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

static
arvore_t *arvore_nova(void) {
    arvore_t *nova = malloc(sizeof(arvore_t));
    if (nova == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }

    nova->raiz = NULL;
    return nova;
}

static
void no_destroi(no_t *no) {
    if (no == NULL) {
        return;
    }

    no_destroi(no->esq);
    no_destroi(no->dir);
    free(no);
}

void arvore_destroi(arvore_t *arvore) {
    no_destroi(arvore->raiz);
    free(arvore);
}

static
no_t *monta_no(const entrada_t *entrada, size_t len, int id) {
    if (id < 0 || id >= len) {
        return NULL;
    }

    no_t *no = no_novo(id);
    no->esq = monta_no(entrada, len, entrada[id].e);
    no->dir = monta_no(entrada, len, entrada[id].d);

    return no;
}

arvore_t *monta_arvore(const entrada_t entrada[], size_t len) {
    arvore_t *arv = arvore_nova();
    arv->raiz = monta_no(entrada, len, 0);
    return arv;
}


size_t altura_no(const no_t *no) {
    if (no == NULL) {
        return 0;
    }

    size_t hdir = altura_no(no->dir);
    size_t hesq = altura_no(no->esq);
    if (hesq > hdir) {
        return hesq + 1;
    } else {
        return hdir + 1;
    }
}

unsigned grau_no(const no_t *no) {
    unsigned esq = (no->esq != NULL);
    unsigned dir = (no->dir != NULL);
    return esq + dir;
}

entrada_t filhos_no(const no_t *no) {
    int esq = (no->esq != NULL)? no->esq->id : VAZIO;
    int dir = (no->dir != NULL)? no->dir->id : VAZIO;

    return (entrada_t) { .e = esq, .d = dir };
}

int no_pai(const no_t *pai) {
    if (pai == NULL) {
        return VAZIO;
    } else {
        return pai->id;
    }
}

const char *tipo_no(const no_t *no, const no_t *pai) {
    if (pai == NULL) {
        return "raiz";
    } else if (grau_no(no) == 0) {
        return "folha";
    } else {
        return "interno";
    }
}

void imprime_no(FILE *stream, const no_t *no, const no_t *pai) {
    if (no == NULL) {
        return;
    }

    unsigned id = no->id;
    int id_pai = no_pai(pai);
    size_t altura = altura_no(no);
    unsigned grau = grau_no(no);
    entrada_t filhos = filhos_no(no);
    const char *tipo = tipo_no(no, pai);

    (void) fprintf(stream,
        "no %u: pai = %d, altura = %zu, grau = %u, filhos = (%d,%d), tipo = %s\n",
        id, id_pai, altura, grau, filhos.e, filhos.d, tipo
    );

    imprime_no(stream, no->esq, no);
    imprime_no(stream, no->dir, no);
}

void imprime_arvore_prefixa(FILE *stream, const arvore_t *arvore) {
    imprime_no(stream, arvore->raiz, NULL);
}
