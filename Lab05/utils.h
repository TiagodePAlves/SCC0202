#ifndef __UTILS_H__
/* Macros utilitárias. */
#define __UTILS_H__

// Lista de atributos da função.
#define attribute(...) __attribute__((__VA_ARGS__))

// Acesso restrito pelo ponteiro.
#define restrict __restrict__

// Inteiro de 8 bits sem sinal.
typedef unsigned char uint8_t;

// Dado booleano
#define bool  uint8_t
// Veradeiro
#define true  1
// Falso
#define false 0

#endif //__UTILS_H__
