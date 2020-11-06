#ifndef __RANDOM_H__
/* Números aleatórios. */
#define __RANDOM_H__

#include <stdlib.h>
#include "utils.h"


// GCC define '__INT_MAX__' e 'RAND_MAX' iguais
#if !defined(__INT_MAX__) || __INT_MAX__ != RAND_MAX || __SIZEOF_INT__ != 4
#   error "versão do compilador inválida"
#endif

/**
 * Gera um número aleatório com 31 bits de entropia,
 * de 0 até INT_MAX.
 */
uint32_t rand_int(void);

/**
 * Gerador de números com probabilidade de meia vida.
 *
 * Isto é, `0` tem 50% de chance, `1` tem 25%, e assim
 * por diante. Em geral, um número 'n' tem "1/2^(n+1)"
 * de probabilidade. O maior número possível é
 * UINT8_MAX.
 */
uint8_t rand_hl(void);

#endif //__RANDOM_H__
