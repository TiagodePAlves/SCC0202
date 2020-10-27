#include "lista.h"
#include <stdlib.h>


// Nó da lista ligada.
typedef struct no {
    /* campos obrigatórios */
    // Valor do nó
    chave_t chave;
    // Tempo em que o nó foi criado
    tempo_t criacao;
    // Algum nó anterior
    struct no *back;
    // O nó seguinte na lista
    struct no *prox;
    /* campo extra */
    // O nó anterior na lista
    struct no *ant;
} no_t;

struct lista {
    no_t *cabeca;
    no_t *final;
    tempo_t total;
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

attribute(nonnull)
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

static attribute(nonnull)
no_t *nth_back(const lista_t *lista, unsigned n) {
    unsigned i = 0;
    for (no_t *ptr = lista->final; ptr != NULL; ptr = ptr->ant) {
        if (i++ >= n) {
            return ptr;
        }
    }
    return lista->cabeca;
}

static attribute(nonnull)
no_t *novo_no(chave_t chave, tempo_t criacao, no_t *back) {
    no_t *novo = malloc(sizeof(no_t));
    if (novo == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    novo->chave = chave;
    novo->criacao = criacao;
    novo->back = back;
    novo->prox = novo->ant = NULL;

    return novo;
}

attribute(nonnull)
void lista_insere(lista_t *lista, chave_t chave, unsigned back) {
    no_t *backward = (back == 0)? NULL : nth_back(lista, back-1);
    no_t *novo = novo_no(chave, lista->total++, backward);

    if (lista_vazia(lista)) {
        lista->cabeca = novo;
    } else {
        novo->ant = lista->final;
        lista->final->prox = novo;
    }
    lista->final = novo;
}

static attribute(nonnull)
no_t *acha_prox(lista_t *lista, chave_t chave) {
    for (no_t *ptr = lista->cabeca; ptr != NULL; ptr = ptr->prox) {
        if (ptr->chave == chave) {
            return ptr;
        }
    }
    return NULL;
}

static attribute(nonnull)
no_t *remove_no(lista_t *lista, no_t *no) {
    if (no->ant == NULL && no->prox == NULL) {
        lista->cabeca = lista->final = NULL;

    } else if (no->ant == NULL) {
        lista->cabeca = no->prox;
        no->prox->ant = NULL;

    } else if (no->prox == NULL) {
        lista->final = no->ant;
        no->ant->prox = NULL;

    } else {
        no->ant->prox = no->prox;
        no->prox->ant = no->ant;
    }

    no_t *prox = no->prox;
    return prox;
}

attribute(nonnull)
void lista_remove(lista_t *lista, chave_t chave) {
    no_t *no = acha_prox(lista, chave);
    lista->total += 1;

    if (no == NULL) {
        return;
    }

    for (no_t *ptr = remove_no(lista, no); ptr != NULL; ptr = ptr->prox) {
        if (ptr->back == no) {
            ptr->back = NULL;
        }
    }
    free(no);
}

static attribute(pure, nonnull)
unsigned posicao_na_lista(const no_t *no) {
    unsigned pos = 0;

    while (no->ant != NULL) {
        no = no->ant;

        pos++;
    }

    return pos;
}

attribute(nonnull(1, 3))
void *lista_iter(const lista_t *lista, void *iter, descricao_t *descricao) {
    const no_t *no;

    if (iter == NULL) {
        no = lista->cabeca;
    } else {
        no = (const no_t *) iter;
    }
    if (no == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }

    descricao->chave = no->chave;
    descricao->criacao = no->criacao;

    if (no->back == NULL) {
        descricao->back = SEM_NO_BACK;
    } else {
        descricao->back = posicao_na_lista(no->back);
    }

    return no->prox;
}
