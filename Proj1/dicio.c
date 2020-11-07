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

void dicio_destroi(dicio_t *dicio) {
    for (no_t *no = dicio->ini[0]; no != NULL;) {
        no_t *prox = no->prox;
        no_destroi(no);
        no = prox;
    }
    free(dicio);
}

typedef struct busca {
    no_t **prox[MAX_NIVEL];
    no_t *eq;
} busca_t;

static inline attribute(const)
busca_t busca_init(void) {
    return (busca_t) {
        .prox = {NULL},
        .eq = NULL,
    };
}

static inline attribute(pure, nonnull)
busca_t dicio_busca_no(const dicio_t *restrict dicio, const char *palavra) {
    busca_t busca = busca_init();

    const no_t **prox = dicio->ini;
    for (size_t i = MAX_NIVEL; i > 0; i--) {
        int cmp = 1;
        while (prox[i-1] != NULL && (cmp = no_str_cmp(prox[i-1], palavra) < 0)) {
            prox = prox[i-1]->prox;
        }
        busca.prox[i-1] = prox + i - 1;
        if (cmp == 0) {
            busca.eq = prox[i-1];
        }
    }
    return busca;
}

bool dicio_insere(dicio_t *restrict dicio, const char *palavra, const char *descricao) {
    busca_t resultado = dicio_busca_no(dicio, palavra);
    if (resultado.eq != NULL) {
        // err...
    }

    no_t *novo = no_novo(palavra, descricao);
    if unlikely(novo == NULL) {
        return false;
    }
    for (size_t i = 0; i < novo->nivel; i++) {
        novo->prox[i] = *resultado.prox[i];
        *resultado.prox[i] = novo;
    }
    return true;
}

// no_t **dicio_busca_no(const dicio_t *restrict dicio, const char *palavra) {
//     for (uint8_t nivel = MAX_NIVEL; nivel > 0; nivel--) {

//     }
// }

// bool dicio_remove(dicio_t *restrict dicio, const char *palavra) {
//     no_t **nos = dicio->ini;
//     for (uint8_t nivel = MAX_NIVEL; nivel > 0; nivel--) {
//         while (nos[nivel-1] != NULL) {
//             if (no_str_cmp(nos[nivel-1], palavra) < 0)
//         }
//         nos[nivel-1] =
//     }
// }
