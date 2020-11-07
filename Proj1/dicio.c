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

attribute(nonnull)
void dicio_destroi(dicio_t *dicio) {
    for (no_t *no = dicio->ini[0]; no != NULL;) {
        no_t *prox = no->prox[0];
        no_destroi(no);
        no = prox;
    }
    free(dicio);
}

typedef struct busca {
    no_t **prox[MAX_NIVEL];
    no_t *eq;
} busca_t;

static inline attribute(pure, nonnull)
busca_t dicio_busca_no(const dicio_t *restrict dicio, const char *palavra, bool para_em_eq) {
    busca_t busca;

    const no_t **prox = (const no_t **) dicio->ini;
    for (size_t i = MAX_NIVEL; i > 0; i--) {
        int cmp = 1;
        while (prox[i-1] != NULL && (cmp = no_str_cmp(prox[i-1], palavra) < 0)) {
            prox = (const no_t **) prox[i-1]->prox;
        }
        busca.prox[i-1] = (no_t **) prox + i - 1;
        if (cmp == 0) {
            busca.eq = (no_t *) prox[i-1];

            if (para_em_eq) {
                return busca;
            }
        }
    }
    return busca;
}

attribute(nonnull)
result_t dicio_insere(dicio_t *restrict dicio, const char *palavra, const char *descricao) {
    busca_t resultado = dicio_busca_no(dicio, palavra, true);
    if unlikely(resultado.eq != NULL) {
        return INVALIDO;
    }

    no_t *novo = no_novo(palavra, descricao);
    if unlikely(novo == NULL) {
        return INESPERADO;
    }
    size_t nivel = novo->nivel;
    for (size_t i = 0; i < nivel; i++) {
        novo->prox[i] = *resultado.prox[i];
        *resultado.prox[i] = novo;
    }
    return OK;
}

attribute(nonnull)
result_t dicio_remove(dicio_t *restrict dicio, const char *palavra) {
    busca_t resultado = dicio_busca_no(dicio, palavra, false);
    if unlikely(resultado.eq == NULL) {
        return INVALIDO;
    }
    no_t *no = resultado.eq;

    size_t nivel = no->nivel;
    for (size_t i = 0; i < nivel; i++) {
        *resultado.prox[i] = no->prox[i];
    }
    no_destroi(no);
    return OK;
}

static inline attribute(const)
const_palavra_t palavra_nao_encontrada(void) {
    return (const_palavra_t) {
        .chave = NULL,
        .descricao = NULL
    };
}

attribute(pure, nonnull)
const_palavra_t dicio_busca(const dicio_t *restrict dicio, const char *palavra) {
    busca_t resultado = dicio_busca_no(dicio, palavra, true);
    if unlikely(resultado.eq == NULL) {
        return palavra_nao_encontrada();
    }
    return no_acessa(resultado.eq);
}
