#ifndef __UTILS_H__
/* Macros e funções utilitárias. */
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

// Aborta a execução com uma mensagem de erro.
#define erro(msg) _error_exit(msg, __FILE__, __LINE__)
_Noreturn
// função auxiliar para a execução do erro
void _error_exit(const char *restrict msg, const char *restrict arquivo, int linha)
attribute(nonnull, noreturn);


#endif //__UTILS_H__
