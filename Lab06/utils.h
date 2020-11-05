#ifndef __UTILS_H__
/* Macros utilitárias. */
#define __UTILS_H__

// Lista de atributos da função.
#define attribute(...) __attribute__((__VA_ARGS__))

#ifdef __GNUC__
// Inteiro de tamanho fixo, sem sinal.
typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;
typedef __uint128_t uint128_t;
// Limites dos inteiros
#define SIZE_BTYE __CHAR_BIT__
#define SIZE_MAX __SIZE_MAX__
#define UINT8_MAX __UINT8_MAX__
#define UINT16_MAX __UINT16_MAX__
#define UINT32_MAX __UINT32_MAX__
#define UINT64_MAX __UINT64_MAX__
#else
#error "compilador inválido"
#endif

// Dado booleano
#define bool  uint8_t
// Veradeiro
#define true  1
// Falso
#define false 0


#define __STR(X) #X
// Transforma entrada em string estática
#define STR(X) __STR(X)


#ifndef NDEBUG
#include <stdio.h>
#include <stdlib.h>

// Imprime erro de assserção.
#define __assert_error(expr) \
    (void) fprintf(stderr, "assertion error: \"%s\"\n", STR(expr))
// Acusa erro se expressão for falsa.
#define assert(expr) do {   \
        if (!(expr)) { \
            __assert_error(expr); \
            abort(); \
        } \
    } while(0)
#else
#define assert(expr) \
    (void) (expr)
#endif

#endif //__UTILS_H__
