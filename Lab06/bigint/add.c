/**
 * Adição e subtração de 'bigint's.
 */
#include "defs.h"


static
// Adição de dois dígitos, atualizando o carry.
digito_t digito_add(digito_t lhs, digito_t rhs, digito_t *carry) {
    digito_t num = lhs + rhs + *carry;
    *carry = num / BASE; // 0 ou 1
    return num % BASE;
}

static
// Subtração de dois dígitos, atualizando o carry.
digito_t digito_sub(digito_t lhs, digito_t rhs, digito_t *carry) {
    // como '-10 <= lhs - rhs - carry < 10', então '0 <= num < 19'
    digito_t num = ((BASE + lhs) - rhs) - *carry;
    // subtração sem problemas
    if (num >= BASE) {
        *carry = 0;
        // remove o 'BASE' que evitava underflow
        return num - BASE;
    // underflow ocorreria
    } else {
        *carry = 1;
        return num;
    }
}

static inline
// Aplica uma operação dígito a dígito em duas listas.
//
// O resultado é escrito novamente em 'lhs', então
// essa lista deve conter pelo menos a mesma quantidade
// de dígitos de 'rhs'.
//
// O carry final, se existir, será alocado em um novo
// dígito com 'alloc_carry'.
//
// Retorna um ponteiro para o último dígito significativo
// do resultado em 'lhs'.
num_t *num_aplica_op(
    num_t *lhs, const num_t *rhs,
    digito_t(*digito_op)(digito_t, digito_t, digito_t *),
    num_t *(*alloc_carry)(digito_t)
) {
    digito_t carry = 0;

    // nó anterior do atual
    num_t *ant = NULL;
    // último dígito significativo (!= 0)
    num_t *ultimo = lhs;
    do {
        // aplica operação (add ou sub)
        lhs->digito = digito_op(lhs->digito, rhs->digito, &carry);
        // atualiza o último significativo
        if (lhs->digito != 0) {
            ultimo = lhs;
        }

        // próximos ponteiros
        ant = lhs;
        lhs = lhs->prox;
        rhs = rhs->prox;
    // assumindo que lhs tem mais (ou igual num.) dígitos que rhs
    } while (rhs != NULL);

    // faz o carry para os dígitos restantes de lhs
    while (carry != 0 && lhs != NULL) {
        lhs->digito = digito_op(lhs->digito, 0, &carry);

        ant = lhs;
        lhs = lhs->prox;
    }
    // um último carry para um novo dígito
    if (carry != 0) {
        ant->prox = alloc_carry(carry);
    }
    // retorna o último significativo
    return ultimo;
}

static inline
// Soma de duas lista de dígitos, sendo que lhs >= rhs.
void num_add(num_t *lhs, const num_t *rhs) {
    // em caso de carry, aloca novo dígito
    (void) num_aplica_op(lhs, rhs, digito_add, num_alloc);
}

static inline
// Subtração de duas lista de dígitos, sendo que lhs >= rhs.
void num_sub(num_t *lhs, const num_t *rhs) {
    // nunca ocorrerá um carry restante
    num_t *zero = num_aplica_op(lhs, rhs, digito_sub, NULL);

    // elimina os zeros à esquerda, que estão depois
    // do dígito mais significativo, sem ser um zero
    if (zero->prox != NULL) {
        num_dealloc(zero->prox);
        zero->prox = NULL;
    }
}

// Soma de dois inteiros com sinal.
bigint_t *bigint_add(const bigint_t *lhs, const bigint_t *rhs) {
    // cópia do maior inteiro, onde ocorre a operação
    bigint_t *maior;
    // ponteiro para o menor (sem copiar)
    const bigint_t *menor;

    // seleção do maior 'bigint'
    int cmp = bigint_cmp(lhs, rhs);
    // caso seja uma subtração com dois iguais
    // já retorna o zero
    if (cmp == 0 && lhs->neg != rhs->neg) {
        return bigint_alloc();
    // lhs < rhs
    } else if (cmp < 0) {
        menor = lhs;
        maior = bigint_copia(rhs);
    // lhs >= rhs
    } else {
        menor = rhs;
        maior = bigint_copia(lhs);
    }

    // mesmo sinal
    if (lhs->neg == rhs->neg) {
        // soma os dígitos e copia o sinal do maior
        num_add(&maior->numero, &menor->numero);
    // sinais diferentes
    } else {
        // subtração, copiando o sinal também
        num_sub(&maior->numero, &menor->numero);
    }
    // resultado da soma
    return maior;
}
