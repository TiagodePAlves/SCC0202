#ifndef __LISTA_H__
#define __LISTA_H__

#include "utils.h"


typedef struct lista lista_t;

typedef struct descricao {
    chave_t chave;
    tempo_t criacao;
} descricao_t;

typedef signed   chave_t;
typedef unsigned tempo_t;

#endif //__LISTA_H__
