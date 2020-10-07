#ifndef __LISTA_H__
/* Lista ligada circular. */
#define __LISTA_H__

#include "utils.h"

/* Tipo para a lista circular de inteiros
 * positivos. Valores negativos são tratados
 * como inválidos.
 */
typedef struct lista lista_t;


/* Gera uma lista nova, inicialmente vazia. */
lista_t *lista_nova(void);

/* Destroi a lista e todos os elementos nela. */
void lista_destroi(lista_t *lista);

/* Teste de habitação da lista. */
bool lista_vazia(const lista_t *lista);

/* Insere número na última posição da lista.
 *
 * NOTA: números devem ser convertíveis pra 'int',
 * i.e. menor que INT_MAX.
 */
void lista_insere(lista_t *lista, unsigned num);

/* Avança uma posição na lista (de forma circular). */
void lista_avanca(lista_t *lista);

/* Remove o primeiro número da lista e retorna o valor.
 * Retorna um negativo se a lista está vazia.
 */
int lista_remove(lista_t *lista);

#endif //__LISTA_H__
