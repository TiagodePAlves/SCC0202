/**
 * Tratamentos gerais de memória do 'bigint'
 * e da lista de dígitos, com funções de
 * alocação, cópia e desalocação.
 */
#include "defs.h"


// Alocação de um novo nó da lista de dígitos.
num_t *num_alloc(digito_t digito) {
    num_t *novo = malloc(sizeof(num_t));
    if (novo == NULL) {
        // encerra o programa em
        // erro de alocação
        exit(EXIT_FAILURE);
        return NULL;
    }
    novo->digito = digito;
    novo->prox = NULL;
    return novo;
}

static
// Deep copy de uma lista de dígitos, mas
// não aloca o dígito menos significativo,
// que vai na struct 'bigint_t'.
num_t num_copia(const num_t *num) {
    // base do número que será retornado
    num_t base = (num_t) {
        .digito = num->digito,
        .prox = NULL
    };

    // ponteiro que acompanha, alocando novos dígitos
    num_t *ptr = &base;
    // percorre a lista, mas pula o primeiro dígito,
    // que já está em 'base'
    for (num = num->prox; num != NULL; num = num->prox) {
        ptr = ptr->prox = num_alloc(num->digito);
    }
    return base;
}

// Desaloca lista de dígitos.
void num_dealloc(num_t *num) {
    do {
        // salva o próximo nó antes do free
        num_t *prox = num->prox;
        free(num);
        num = prox;
    } while (num != NULL);
}

// Alocação de um 'bigint' zerado.
bigint_t *bigint_alloc(void) {
    bigint_t *novo = malloc(sizeof(bigint_t));
    if (novo == NULL) {
        // encerra em caso de erro
        exit(EXIT_FAILURE);
        return NULL;
    }
    // monta como '+0'
    novo->numero = (num_t) {
        .digito = 0,
        .prox = NULL
    };
    novo->neg = false;

    return novo;
}

// Cópia de um 'bigint'.
bigint_t *bigint_copia(const bigint_t *num) {
    bigint_t *novo = bigint_alloc();
    // cópia dos dígitos
    novo->numero = num_copia(&num->numero);
    // e do sinal
    novo->neg = num->neg;
    return novo;
}

void bigint_dealloc(bigint_t *num) {
    if (num->numero.prox != NULL) {
        // desaloca os outros dígitos
        num_dealloc(num->numero.prox);
    }
    // desaloca o 'bigint' e o dígito
    // mais significativo
    free(num);
}
