#include "dicio.h"
#include <stdlib.h>

#include "dicio/no.c"

typedef struct dicio {
    // Ponteiros iniciais da lista,
    // em cada nível.
    no_t *ini[MAX_NIVEL];
} dicio_t;


dicio_t *dicio_novo(void) {
    return calloc(1, sizeof(dicio_t));
}

static inline
void f() {
}

// void dicio_insere(dicio_t *dicio, const char *palavra, const char *descricao) {
//     no_t *no = no_novo(palavra, descricao);
//     if unlikely(no == NULL) {
//         // err...
//     }

//     for (uint8_t i = 0; i < no->nivel; i++) {

//     }
// }
