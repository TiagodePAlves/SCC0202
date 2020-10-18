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

    if (lista_vazia(lista)) {
        lista->cabeca = novo;
    } else {
        lista->final->prox = novo;
    }
    lista->final = novo;
    lista->total += 1;
    lista->tamanho += 1;
}

no_t *remove_prox(lista_t *lista, chave_t chave) {
    if (lista_vazia(lista)) {
        return NULL;

    } else if (lista->cabeca->chave == chave) {
        no_t *no = lista->cabeca;
        lista->cabeca = no->prox;
        if (lista->cabeca == NULL) {
            lista->final = NULL;
        }
        return no;
    }

    for (no_t *ptr = lista->cabeca; ptr->prox != NULL; ptr = ptr->prox) {
        if (ptr->prox->chave == chave) {
            no_t *no = ptr->prox;

            ptr->prox = no->prox;
            if (ptr->prox == NULL) {
                lista->final = ptr;
            }
            return no;
        }
    }
    return NULL;
}

void lista_remove(lista_t *lista, chave_t chave) {
    no_t *removido = remove_prox(lista, chave);
    lista->total += 1;

    if (removido == NULL) {
        return;
    }
    lista->tamanho -= 1;

    for (no_t *ptr = lista->cabeca; ptr != NULL; ptr = ptr->prox) {
        if (ptr->back == removido) {
            ptr->back = NULL;
        }
    }
    free(removido);
}
