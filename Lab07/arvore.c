#include "arvore.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct no {
    int id;
    struct no *esq;
    struct no *dir;
} no_t;

struct arvore {
    no_t raiz;
};
