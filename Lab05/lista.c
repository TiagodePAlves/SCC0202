#include "lista.h"
#include <stdlib.h>


typedef struct no {
    chave_t chave;
    tempo_t criacao;
    struct no *back;
    struct no *prox;
} no_t;

struct lista {
    no_t *cabeca;
    no_t *final;
    tempo_t total;
    unsigned tamanho;
};


lista_t *lista_nova(void) {
    lista_t *nova = malloc(sizeof(lista_t));
    if (nova == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }

    nova->cabeca = NULL;
    nova->final = NULL;
    nova->total = 0;
    return nova;
}

void lista_destroi(lista_t *lista) {
    for (no_t *ptr = lista->cabeca; ptr != NULL;) {
        no_t *prox = ptr->prox;
        free(ptr);
        ptr = prox;
    }

    free(lista);
}

extern inline attribute(pure, nonnull)
bool lista_vazia(const lista_t *lista) {
    return lista->cabeca == NULL;
}


no_t *nth_front(const lista_t *lista, unsigned n) {
    unsigned i = 0;
    for (no_t *ptr = lista->cabeca; ptr != NULL; ptr = ptr->prox) {
        if (i++ >= n) {
            return ptr;
        }
    }
    return lista->final;
}

no_t *nth_back(const lista_t *lista, unsigned n) {
    unsigned front = (lista->tamanho >= n)? lista->tamanho - n : 0;
    return nth_front(lista, front);
}

void lista_insere(lista_t *lista, chave_t chave, unsigned back) {
    no_t *novo = malloc(sizeof(no_t));
    if (novo == NULL) {
        exit(EXIT_FAILURE);
        return;
    }
    novo->chave = chave;
    novo->criacao = lista->total;
    novo->prox = NULL;

    if (back > 0) {
        novo->back = nth_back(lista, back-1);
    } else {
        novo->back = NULL;
    }

    lista->final->prox = novo;
    lista->final = novo;
    lista->total += 1;
    lista->tamanho += 1;
}
