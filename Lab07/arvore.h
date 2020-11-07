#ifndef __ARVORE_H__
#define __ARVORE_H__

#include <stdlib.h>
#include <stdio.h>


#define VAZIO -1

typedef struct entrada {
    int e, d;
} entrada_t;

typedef struct arvore arvore_t;

arvore_t *monta_arvore(const entrada_t *entrada, size_t len);

void imprime_arvore(const arvore_t *arvore);

#endif //__ARVORE_H__