/**
 * Comparação de 'bigint's.
 *
 * Retornos:
 *  - negativo: 'lhs < rhs'
 *  - zero    : 'lhs == rhs'
 *  - positivo: 'lhs > rhs'
 */
#include "defs.h"


static inline
// Comparação de dois dígitos.
int digito_cmp(digito_t lhs, digito_t rhs) {
    return ((int) lhs) - ((int) rhs);
}

static
// Comparação de listas de dígitos.
int num_cmp(const num_t *lhs, const num_t *rhs) {
    int cmp = 0;

    do {
        // compara os dígitos atuais
        int res = digito_cmp(lhs->digito, rhs->digito);
        // se eles forem iguais, mantém a comparação
        // dos dígitos menores
        if (res != 0) {
            cmp = res;
        }
        // próximos dígitos
        lhs = lhs->prox;
        rhs = rhs->prox;
    } while (lhs != NULL && rhs != NULL);

    // fim de ambas as lista
    if (lhs == NULL && rhs == NULL) {
        return cmp;
    // ou se uma terminou antes da outra
    } else if (rhs == NULL) {
        return 1;
    } else {
        return -1;
    }
}

static inline
bool eh_zero(const bigint_t *num) {
    return num->numero.digito == 0 && num->numero.prox == NULL;
}

// Comparação dos 'bigint'.
int bigint_cmp(const bigint_t *lhs, const bigint_t *rhs) {
    // ambos são zero (ignorando o sinal)
    if (eh_zero(lhs) && eh_zero(rhs)) {
        return 0;
    // ambos positivos
    } else if (!lhs->neg && !rhs->neg) {
        return num_cmp(&lhs->numero, &rhs->numero);
    // rhs < 0 < lhs
    } else if (!lhs->neg) {
        return 1;
    // lhs < 0 < rhs
    } else if (!rhs->neg) {
        return -1;
    // ambos negativos
    } else {
        // inverte o resultado da comparação das listas
        return -num_cmp(&lhs->numero, &rhs->numero);
    }
}
