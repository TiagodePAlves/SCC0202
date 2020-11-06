/**
 * SCC0202 - 2020 2S
 *
 * Nome: William Maehara
 * Código: 6792263
 * * * * * * * * * * * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigint.h"
#include "utils.h"


// Tipos de comparação possíveis.
typedef enum comparacao {
    BIG, SML, EQL
} comparacao_t;

static
// Compara dois 'bigint's, dado um tipo de comparação,
// e desaloca eles.
void compara(bigint_t *a, bigint_t *b, comparacao_t tipo) {
    // comparação dos inteiros
    int cmp = bigint_cmp(a, b);

    // resultado em relação ao esperado
    bool res = false;
    switch (tipo) {
        // comparação de a > b
        case BIG:
            res = (cmp > 0);
            break;
        // a == b
        case EQL:
            res = (cmp == 0);
            break;
        // a < b
        case SML:
            res = (cmp < 0);
    }
    // mostrando o resultado
    (void) fputc_unlocked(res? '1' : '0', stdout);

    // desaloca inteiros
    bigint_dealloc(a);
    bigint_dealloc(b);
}

static
// Soma dois 'bigint's, mostra o resultaod e desaloca eles.
void soma(bigint_t *a, bigint_t *b) {
    bigint_t *res = bigint_add(a, b);
    // mostra com a função da 'bigint.h'
    bigint_show(stdout, res);

    // desaloca inteiros
    bigint_dealloc(a);
    bigint_dealloc(b);
    bigint_dealloc(res);
}

int main(void) {
    // linhas de entrada
    size_t n;
    int rv = scanf("%zu\n", &n);
    if (rv < 1) return EXIT_FAILURE;

    for (size_t i = 0; i < n; i++) {
        // operação a ser executada
        char op[10];
        rv = scanf("%9s", op);
        if (rv < 1) return EXIT_FAILURE;

        // leitura dos inteiros
        bigint_t *arg1 = bigint_read(stdin);
        bigint_t *arg2 = bigint_read(stdin);

        (void) printf("Resultado %zu: ", i + 1);
        // faz a operação e apresenta o resultado
        if (strcmp(op, "sum") == 0) {
            soma(arg1, arg2);
        } else if (strcmp(op, "big") == 0) {
            compara(arg1, arg2, BIG);
        } else if (strcmp(op, "eql") == 0) {
            compara(arg1, arg2, EQL);
        } else if (strcmp(op, "sml") == 0) {
            compara(arg1, arg2, SML);
        }
        // encerra a linha de saída
        (void) printf("\n");
    }

    return EXIT_SUCCESS;
}
