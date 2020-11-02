#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdlib.h>
#include "utils.h"


uint8_t __max_hl(void) attribute(const);
/**
 * Maior valor possível de `rand_hl`.
 * Equivalente a `ceil(log2(RAND_MAX))`.
 */
#define MAX_HL __max_hl()

/**
 * Gerador de números com probabilidade de meia vida.
 *
 * Isto é, 0 tem 50% de chance, 1 tem 25%, e assim
 * por diante. Em geral, um número 'n' tem '1/2^(n+1)'
 * de probabilidade. O maior número possível é
 * 'MAX_HL', que em geral é 32.
 */
uint8_t rand_hl(void);

#endif //__RANDOM_H__
