#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bigint.h"
#include "utils.h"


typedef enum comparacao {
    BIG, SML, EQL
} comparacao_t;

static
void compara(bigint_t *a, bigint_t *b, comparacao_t tipo) {
    int cmp = bigint_cmp(a, b);

    bool res;
    switch (tipo) {
        case BIG:
            res = (cmp > 0);
            break;
        case EQL:
            res = (cmp == 0);
            break;
        case SML:
            res = (cmp < 0);
    }
    (void) printf("%hhu\n", res);

    bigint_dealloc(a);
    bigint_dealloc(b);
}

static
void soma(bigint_t *a, bigint_t *b) {
    bigint_t *res = bigint_add(a, b);
    bigint_show(stdout, res);
    printf("\n");

    bigint_dealloc(a);
    bigint_dealloc(b);
    bigint_dealloc(res);
}

int main(void) {
    size_t n;
    int rv = scanf("%zu\n", &n);
    if (rv < 1) {
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < n; i++) {
        char op[10];
        rv = scanf("%9s", op);
        if (rv < 1) {
            return EXIT_FAILURE;
        }

        bigint_t *arg1 = bigint_read(stdin);
        bigint_t *arg2 = bigint_read(stdin);

        (void) printf("Resultado %zu: ", i);
        if (strcmp(op, "sum") == 0) {
            soma(arg1, arg2);
        } else if (strcmp(op, "big") == 0) {
            compara(arg1, arg2, BIG);
        } else if (strcmp(op, "eql") == 0) {
            compara(arg1, arg2, EQL);
        } else if (strcmp(op, "sml") == 0) {
            compara(arg1, arg2, SML);
        } else {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
