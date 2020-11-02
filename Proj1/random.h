#ifndef __RANDOM_H__
/* Números aleatórios. */
#define __RANDOM_H__

#include <stdlib.h>
#include "utils.h"


// GCC define '__INT_MAX__' e 'RAND_MAX' iguais
#if defined(__INT_MAX__) && __INT_MAX__ == RAND_MAX
#   if __SIZEOF_INT__ == 8
    /* Maior valor possível de `rand_hl`. */
#       define MAX_RHL 63
#   elif __SIZEOF_INT__ == 4
    /* Maior valor possível de `rand_hl`. */
#       define MAX_RHL 31
#   elif __SIZEOF_INT__ == 2
    /* Maior valor possível de `rand_hl`. */
#       define MAX_RHL 15
#   else
#       error "largura de 'int' inválida"
#   endif
#else
#   error "versão do compilador inválida"
#endif

/**
 * Gerador de números com probabilidade de meia vida.
 *
 * Isto é, `0` tem 50% de chance, `1` tem 25%, e assim
 * por diante. Em geral, um número 'n' tem "1/2^(n+1)"
 * de probabilidade. O maior número possível é
 * 'MAX_HL', que em geral é 31.
 */
uint8_t rand_hl(void);

#endif //__RANDOM_H__
