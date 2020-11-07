#include <stdlib.h>
#include <stdio.h>
#include "arvore.h"


entrada_t *entradas_init(size_t len) {
    entrada_t *entradas = malloc(len * sizeof(entrada_t));
    if (entradas == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }

    for (size_t i = 0; i < len; i++) {
        entradas[i].e = VAZIO;
        entradas[i].d = VAZIO;
    }
    return entradas;
}

entrada_t *le_entradas(size_t len) {
    entrada_t *entradas = entradas_init(len);

    for (size_t i = 0; i < len; i++) {
        size_t id;
        int esq, dir;
        int rv = scanf("%zu %d %d\n", &id, &esq, &dir);
        if (rv < 0) {
            return entradas;
        } else if (rv < 3) {
            continue;
        }

        if (id < len) {
            entradas[id].e = esq;
            entradas[id].d = dir;
        }
    }
    return entradas;
}

int main(void) {
    size_t len;
    int rv = scanf("%zu\n", &len);
    if (rv < 1) {
        return EXIT_FAILURE;
    }

    entrada_t *entradas = le_entradas(len);

    arvore_t *arvore = monta_arvore(entradas, len);
    free(entradas);

    imprime_arvore_prefixa(stdout, arvore);

    arvore_destroi(arvore);
    return EXIT_SUCCESS;
}
