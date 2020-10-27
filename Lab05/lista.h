#ifndef __LISTA_H__
/* Lista ligada e suas operações. */
#define __LISTA_H__

#include "utils.h"

// Marcador de objeto da lista sem nó backward.
#define SEM_NO_BACK 0xFFFFFFFFU

// Lista duplamente ligada.
typedef struct lista lista_t;

// Chave de cada elemento da lista.
typedef signed   chave_t;
// Tipo para marcador de tempo (em operações).
typedef unsigned tempo_t;


// Descrição de um elemento da lista.
typedef struct descricao {
    chave_t chave;
    tempo_t criacao;
    unsigned back;
} descricao_t;


/* Cria uma lista nova.
 * Nunca retorna `NULL`.
 */
lista_t *lista_nova(void);

/* Desaloca uma lista e todos
 * os seus elementos.
 */
void lista_destroi(lista_t *lista)
attribute(nonnull);

/* Teste de lista vazia. */
bool lista_vazia(const lista_t *lista)
attribute(pure, nonnull);

/* Cria novo elemento na lista com a `chave` dada
 * e referenciando um nó `back` posições atrás.
 */
void lista_insere(lista_t *lista, chave_t chave, unsigned back)
attribute(nonnull);

/* Remove o primeiro nó da lista
 * com a dada `chave`.
 */
void lista_remove(lista_t *lista, chave_t chave)
attribute(nonnull);

/*  Iteração sobre os elementos de uma lista não vazia.
 *
 * O ponteiro `iter` marca a posição na lista, de forma
 * similar à função POSIX `strtok_r'. Para iniciar a
 * iteração, `iter` deve ser `NULL`.
 *
 * A função sempre retorna o próximo marcador iter,
 * encerrando quando retorna `NULL`.
 *
 * `descricao` guarda os detalhes do elemento, caso
 * a função não retorne nulo.
 *
 * # Exemplo
 *
 * ```
 * descricao dc;
 * for (void *it = lista_iter(lista, NULL, &dc); it != NULL; it = lista_iter(lista, it, &dc)) {
 *     // usa a descrição `dc` do elemento
 *     // lembrando que `dc.back` pode ser `SEM_NO_BACK`
 * }
 * ```
 */
void *lista_iter(const lista_t *lista, void *iter, descricao_t *descricao)
attribute(nonnull(1, 3));


#endif //__LISTA_H__
