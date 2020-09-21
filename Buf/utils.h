#ifndef __UTILS_H__
/* Macros utilitárias. */
#define __UTILS_H__

// https://gcc.gnu.org/onlinedocs/gcc-4.8.5/cpp/Stringification.html
#define __STRINGIFY(token) #token
/* Transforma um token qualquer em uma string literal. */
#define STR(token) __STRINGIFY(token)


/* Declaração de atributos pro compilador. */
#define attribute(...) __attribute__((__VA_ARGS__))


/* Máximo de dois inteiro. */
#define max(x, y) \
    (((x) >= (y))? (x) : (y))

/* Mínimo de dois inteiros. */
#define min(x, y) \
    (((x) <= (y))? (x) : (y))


/* Tipo representando 1 byte de dados. */
typedef unsigned char byte_t;

// dicas de branching
/* Indicação de braching que é provável de ocorrer. */
#define likely(x)   __builtin_expect((x), 1)
/* Indica branching improvável, comum em casos de erro. */
#define unlikely(x) __builtin_expect((x), 0)

/* Checando problemas de alocação. */
#define ALLOC_CHECK(ptr, funcao, tam) \
    if (unlikely((ptr) == NULL)) { \
        return _alloc_err(__FILE__, STR(funcao), __LINE__, (tam)); \
    }

// funções auciliares das macros
#include <stdlib.h> // or <stddef.h>

void *_alloc_err(const char *, const char *, size_t, size_t)
attribute(noinline, noreturn);

#endif/*__UTILS_H__*/
