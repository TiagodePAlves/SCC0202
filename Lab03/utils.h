#ifndef __UTILS_H__
/* Macros utilitárias. */
#define __UTILS_H__

// Dado booleano
#define bool  unsigned char
// Veradeiro
#define true  1
// Falso
#define false 0


// Lista de atributos da função.
#define attribute(...) __attribute__((__VA_ARGS__))

// Acesso restrito pelo ponteiro.
#define restrict __restrict__

// Inteiro de 8 bits sem sinal.
typedef unsigned char uint8_t;
// pode ter outra largura, mas é pelo menos 8 bits

#endif //__UTILS_H__
