#ifndef __NO_H__
/**
 * Nó da skip list.
 *
 * Esse header declara as funções principais,
 * mas o arquivo "no.c" será incluido na
 * unidade de compilação da skip lists.
 */
#define __NO_H__

#include "../random.h"
#include "../dicio.h"
#include <stdlib.h>
#include <string.h>


// Uma palavra guardada no nó.
typedef struct palavra {
    // A palavra propriamente.
    // Pode ser NULL, se a palavra cabe completamente
    // em `no_t.ini`, ou seja, a palavra tem menos de
    // `EXTRA_PADDING` caracteres.
    char *chave;
    // A descrição do palavra.
    char *descricao;
} palavra_t;

// Maior nível possível.
#define MAX_NIVEL (MAX_HL + 1)

/**
 * Padding da struct normalmente inutilizado.
 *
 * Nesse caso, será usado para guardar os primeiros
 * caracteres da palavra, para agilizar o acesso.
 */
#define EXTRA_PADDING \
    sizeof(struct {palavra_t p; uint8_t n;}) \
        - (sizeof(palavra_t) + sizeof(uint8_t))

// Nó da lista
typedef struct no {
    // A palavra realmente.
    palavra_t palavra;
    // Usa o espaço de padding para guardar os
    // primeiros caracteres da palavra, evitando
    // acessos desnecessários e aproveitando o cache.
    char ini[EXTRA_PADDING];
    // Nível desse nó.
    uint8_t nivel;
    // Próximos nós.
    struct no *prox[];
} no_t;


static inline
/**
 * Aloca um novo nó.
 * Retorna `NULL` se ocorrer algum erro de alocação.
 */
no_t *no_novo(const char *chave, const char *descricao)
attribute(nonnull);

static inline
/**
 * Desaloca um nó.
 */
void no_destroi(no_t *no)
attribute(nonnull);

static inline
/**
 * Acesso da palavra do nó.
 */
const_palavra_t no_acessa(const no_t *no)
attribute(pure, nonnull);

static inline
/**
 * Comparação da chave de um nó com uma string.
 *
 * Retorno
 *  - negativo: `no->palavra.chave` < `str`
 *  - zero    : `no->palavra.chave` == `str`
 *  - positivo: `no->palavra.chave` > `str`
 */
int no_str_cmp(const no_t *no, const char *str)
attribute(pure, nonnull);

static inline
/**
 * Comparação da chave de `lhs` com `rhs`.
 *
 * Retorno
 *  - negativo: `lhs` < `rhs`
 *  - zero    : `lhs` == `rhs`
 *  - positivo: `lhs` > `rhs`
 */
int no_cmp(const no_t *lhs, const no_t *rhs)
attribute(pure, nonnull);

#endif //__NO_H__
