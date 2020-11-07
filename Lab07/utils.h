#ifndef __UTILS_H__
/* Macros utilitárias. */
#define __UTILS_H__


#ifdef __GNUC__
    // Inteiro de tamanho fixo, sem sinal.
    typedef __UINT8_TYPE__ uint8_t;
#else
#   error "compilador inválido"
#endif

// Dado booleano
#define bool  uint8_t
// Veradeiro
#define true  1
// Falso
#define false 0

#endif //__UTILS_H__
