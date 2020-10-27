#ifndef __LISTA_H__
#define __LISTA_H__

#include "utils.h"

#define SEM_NO_BACK 0xFFFFFFFFU

// Lista duplamente ligada.
typedef struct lista lista_t;

typedef signed   chave_t;
typedef unsigned tempo_t;

typedef struct descricao {
    chave_t chave;
    tempo_t criacao;
    unsigned back;
} descricao_t;


lista_t *lista_nova(void);

void lista_destroi(lista_t *lista)
attribute(nonnull);

bool lista_vazia(const lista_t *lista)
attribute(pure, nonnull);

void lista_insere(lista_t *lista, chave_t chave, unsigned back)
attribute(nonnull);

void lista_remove(lista_t *lista, chave_t chave)
attribute(nonnull);

void *lista_iter(const lista_t *lista, void *iter, descricao_t *descricao)
attribute(nonnull(1, 3));


#endif //__LISTA_H__
