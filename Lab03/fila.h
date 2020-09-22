#ifndef __FILA_H__
/* Implementação da fila. */
#define __FILA_H__

#include "utils.h"


// Estrutura para a fila.
typedef struct fila fila_t;
// Tipo dos elementos da fila.
typedef void *elem_t;


// Aloca nova fila no heap e retorna o endereço dela.
// Aborta em caso de erro de alocação, nunca retornando null.
fila_t *fila_nova(void);

// Aloca um vetor de novas filas e retorna o endereço do vetor.
// Aborta em caso de erro de alocação, nunca retornando null.
// Um vetor de tamanho zero é considerado inválido também.
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
