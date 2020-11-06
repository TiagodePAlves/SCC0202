#ifndef __BIGINT_DEFS__
/* Tipos e funções internas do 'bigint'. */
#define __BIGINT_DEFS__

#include "../bigint.h"
#include "../utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Lista ligada dos dígitos.
typedef struct num num_t;
// Inteiro que armazena um dígito decimal.
typedef uint8_t digito_t;

// Base decimal utilizada para o 'bigint'.
#define BASE    ((digito_t) 10)

// O inteiro de largura dinâmica é basicamente
// uma lista ligada de dígitos BCD.
// O primeiro elemento é o menos significativo.
struct num {
    digito_t digito;
    num_t *prox;
};

// O 'bigint_t' contém a cabeça da lista,
// já que deve sempre existir ao menos um
// dígito, e o sinal do número.
// Não podem existir zeros à esquerda.
struct bigint {
    // lista com os dígitos
    num_t numero;
    // se o número é negativo
    bool neg;
};

// Alocação de um nó da lista de números,
// com o dado dígito.
num_t *num_alloc(digito_t digito);
// Desaloca um nó da lista, e todos os nós
// subsequentes.
void num_dealloc(num_t *num);

#endif //__BIGINT_DEFS__
