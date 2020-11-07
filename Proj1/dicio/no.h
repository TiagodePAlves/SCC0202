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
    char *chave;
    // A descrição do palavra.
    char *descricao;
} palavra_t;

// Maior nível possível.
#define MAX_NIVEL (MAX_HL + 1)

// Nó da lista
typedef struct no {
    // A palavra realmente.
    palavra_t palavra;
    // Primeiro caracter da palavra.
    char ini;
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
 * Altera a descrição de um nó.
 * Retorna se a operação obteve sucesso.
 */
bool no_altera_descricao(no_t *no, const char *descricao)
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
int no_cmp(const no_t *no, const char *str)
attribute(pure, nonnull);

#endif //__NO_H__
