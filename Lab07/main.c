/**
 * SCC0202 - 2020 2S
 *
 * Nome: William Maehara
 * Código: 6792263
 * * * * * * * * * * * */
#include <stdlib.h>
#include <stdio.h>
#include "arvore.h"


static
/**
 * Inicializa o vetor de entradas marcando todos os nós
 * como folhas.
 */
entrada_t *entradas_init(size_t len) {
    entrada_t *entradas = malloc(len * sizeof(entrada_t));
    // erro de alocação
    if (entradas == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    // marca os nós como folhas
    for (size_t i = 0; i < len; i++) {
        entradas[i].e = VAZIO;
        entradas[i].d = VAZIO;
    }
    return entradas;
}

static
/**
 * Leitura da entrada que representa a árvore.
 */
entrada_t *le_entradas(size_t len) {
    entrada_t *entradas = entradas_init(len);

    // id do nó lido
    size_t id;
    // ids dos filhos do nó
    int esq, dir;
    // resultado do scanf
    int rv;
    // leitura até o EOF
    while ((rv = scanf("%zu %d %d\n", &id, &esq, &dir)) >= 0) {
        // linha incompleta
        if (rv < 3) {
            continue;
        }
        // escreve os dados no id certo
        if (id < len) {
            entradas[id].e = esq;
            entradas[id].d = dir;
        }
    }
    return entradas;
}

/* MAIN */
int main(void) {
    size_t len;
    // tamanho do vetor de entrada
    int rv = scanf("%zu\n", &len);
    if (rv < 1) {
        return EXIT_FAILURE;
    }

    // leitura das entradas
    // nós incompletos são marcados como folhas
    entrada_t *entradas = le_entradas(len);

    // monta a árvore e libera o vetor de entradas
    arvore_t *arvore = monta_arvore(entradas, len);
    free(entradas);

    // impressão da árvore
    imprime_arvore_prefixa(stdout, arvore);

    arvore_destroi(arvore);
    return EXIT_SUCCESS;
}
