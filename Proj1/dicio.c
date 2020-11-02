#include "dicio.h"
#include <stdlib.h>

#define NIVEL_MAX 32


typedef struct palavra {
    char *chave;
    char *descricao;
    size_t nivel;
    palavra_t *prox[];
} palavra_t;

typedef struct dicio {
    palavra_t *ini[NIVEL_MAX];
} dicio_t;


dicio_t *dicio_novo(void) {
    return NULL;
}
