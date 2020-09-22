#ifndef NDEBUG
// Usa a biblioteca apenas no editor de texto.
#include <stdbool.h>
#else
// mas no programa enviado, usa uma
// implementação simples do <stdbool.h>

#ifndef __BOOL_H__
/* Dados booleanos. */
#define __BOOL_H__

// #ifdef NDEBUG
// Dado booleano
#define bool  _Bool

// Veradeiro
#define true  1
// Falso
#define false 0

#endif //__BOOL_H__
#endif //NDEBUG
