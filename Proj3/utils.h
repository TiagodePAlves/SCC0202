#ifndef __UTILS_H__
/* Macros utilitárias. */
#define __UTILS_H__


// Lista de atributos da função.
#define attribute(...) __attribute__((__VA_ARGS__))


// Inteiro de tamanho fixo, sem sinal.
typedef __UINT8_TYPE__ uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
typedef __UINT64_TYPE__ uint64_t;
// Limite dos inteiros.
#define UINT8_MAX __UINT8_MAX__
#define UINT16_MAX __UINT16_MAX__
#define UINT32_MAX __UINT32_MAX__
#define UINT64_MAX __UINT64_MAX__


// Marcador de branch provável.
#define likely(x) \
    (__builtin_expect((x), 1))
// Marcador de branch improvável.
#define unlikely(x) \
    (__builtin_expect((x), 0))


#define _STR_(X) #X
// Macro de transformação em texto.
#define STR(X) _STR_(X)


// Dado booleano
typedef _Bool bool;
// Veradeiro
#define true  1
// Falso
#define false 0

#endif //__UTILS_H__
