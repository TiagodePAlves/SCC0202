#ifndef __BIGINT_H__
/* Inteiro de largura dinâmica. */
#define __BIGINT_H__

#include <stdio.h>

// Tipo do inteiro de largura dinâmica.
typedef struct bigint bigint_t;

/**
 * Cria um 'bigint' com valor zero.
 *
 * Encerra o programa em erro de alocação.
 */
bigint_t *bigint_alloc(void);
/**
 * Cria uma cópia de um 'bigint'.
 *
 * Encerra o programa em erro de alocação.
 */
bigint_t *bigint_copia(const bigint_t *num);
/**
 * Desaloca um 'bigint'.
 */
void bigint_dealloc(bigint_t *num);
/**
 * Comparação de dois inteiros.
 *
 * Retorna:
 *  - negativo:  se 'lhs < rhs'
 *  - zero    :  se 'lhs == rhs'
 *  - positivo:  se 'lhs > rhs'
 */
int bigint_cmp(const bigint_t *lhs, const bigint_t *rhs);
/**
 * Soma dois inteiros. O resultado é alocado em
 * um novo 'bigint.
 *
 * Encerra o programa em erro de alocação.
 */
bigint_t *bigint_add(const bigint_t *lhs, const bigint_t *rhs);
/**
 * Leitura de um 'bigint' decimal em um file stream.
 *
 * Espaços antes do número são ignorados, assim como zeros
 * à esquerda. Faz a leitura de um caracter a mais, após o
 * número, que é ignorado.
 *
 * Encerra o programa em erro de alocação.
 */
bigint_t *bigint_read(FILE *stream);
/**
 * Escrita de um 'bigint' decimal em um file stream.
 *
 * Encerra o programa em erro de alocação.
 */
void bigint_show(FILE *stream, const bigint_t *num);

#endif //__BIGINT_H__
