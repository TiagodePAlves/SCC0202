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
#define SIZE_MAX __SIZE_MAX__
#define UINT8_MAX __UINT8_MAX__
#define UINT16_MAX __UINT16_MAX__
#define UINT32_MAX __UINT32_MAX__
#define UINT64_MAX __UINT64_MAX__
#else
#error "compilador inválido"
#endif

// Marcador de branch provável.
#define likely(x) \
    (__builtin_expect((x), 1))
// Marcador de branch improvável.
#define unlikely(x) \
    (__builtin_expect((x), 0))

// Dado booleano
#define bool  uint8_t
// Veradeiro
#define true  1
// Falso
#define false 0

#endif //__UTILS_H__
