#ifndef __ARVORE_H__
/**
 * Árvores binárias.
 */
#define __ARVORE_H__

#include <stdlib.h>
#include <stdio.h>


/* Id de um nó vazio. */
#define VAZIO -1

/* Representação de um nó pela entrada padrão. */
typedef struct entrada {
    // Ids dos nós esquerdo e direito.
    int e, d;
} entrada_t;

/* Representação da árvore. */
typedef struct arvore arvore_t;


/**
 * Monta uma árvore a partir do vetor de entradas.
 *
 * O id de cada entrada é a posição dela no vetor e
 * ela deve conter o id dos nós filhos. Ids que não
 * são posições válidas na lista serão tratados como
 * filhos vazios. A raiz é o id zero.
 *
 * Encerra o programa em erro de alocação.
 */
arvore_t *monta_arvore(const entrada_t entrada[], size_t len);

/**
 * Desalocação da árvore e de todos os seus nós.
 */
void arvore_destroi(arvore_t *arvore);

/**
 * Imprime todos os nós da árvore, cada um em uma linha,
 * varrendo em ordem préfixa.
 */
void imprime_arvore_prefixa(FILE *stream, const arvore_t *arvore);

#endif //__ARVORE_H__