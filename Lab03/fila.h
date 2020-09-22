#ifndef __FILA_H__
/* Implementação da fila. */
#define __FILA_H__

#include "bool.h"


// Estrutura para a fila.
typedef struct fila fila_t;
// Tipo dos elementos da fila.
typedef void *elem_t;


// Lista de atributos da função.
#define attribute(...) __attribute__((__VA_ARGS__))
// Acesso restrito pelo ponteiro.
#define restrict __restrict__

// Aloca nova fila no heap e retorna o endereço dela.
// Aborta em caso de erro de alocação, nunca retornando null.
fila_t *fila_nova(void);

// Desaloca toda memória da fila com o `destrutor` para cada
// elemento.
// Destrutor pode ser NULL, indicando que não há necessidade
// de cuidar da memória de cada elemento.
void fila_destroi(fila_t *fila, void (*destrutor)(elem_t))
attribute(nonnull(1));

// Teste de fila vazia.
bool fila_vazia(const fila_t *fila)
attribute(pure, nonnull);

// Teste de fila cheia.
bool fila_cheia(const fila_t *fila)
attribute(pure, nonnull);

// Insere novo elemento no final da fila.
// Aborta em caso de erro ao aumentar o buffer.
void fila_insere(fila_t *fila, elem_t elem)
attribute(nonnull);

// Tenta remover um elemento do início da fila.
// Quando a lista está vazia, é retornado falso e não
// ocorre nenhum tipo de acesso em `elem`.
bool fila_remove(fila_t *restrict fila, elem_t *restrict elem)
attribute(nonnull);

#endif//__FILA_H__
