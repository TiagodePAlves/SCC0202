#ifndef __FILA_H__
/* Implementação de uma fila (FIFO) simples. */
#define __FILA_H__

#include "utils.h"
#include <stdlib.h>


// Estrutura para a fila.
typedef struct fila fila_t;
// Tipo dos elementos da fila.
typedef void *elem_t;


// Aloca nova fila no heap e retorna o endereço dela.
// Assume que não ocorre erro de alocação.
fila_t *fila_nova(void);

// Aloca um vetor de novas filas e retorna o endereço do vetor.
// Assume que não ocorre erro de alocação.
fila_t *fila_vetor_novo(size_t N);

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

// Acesso em um vetor de filas.
fila_t *fila_vetor_pos(const fila_t *fila, size_t pos)
attribute(const);

// Insere novo elemento no final da fila.
// Assume que não ocorre erro de alocação.
void fila_insere(fila_t *fila, elem_t restrict elem)
attribute(nonnull);

// Tenta remover um elemento do início da fila.
// Quando a lista está vazia, é retornado falso e não
// ocorre nenhum tipo de acesso em `elem`.
bool fila_remove(fila_t *restrict fila, elem_t restrict *elem)
attribute(nonnull);

#endif//__FILA_H__
