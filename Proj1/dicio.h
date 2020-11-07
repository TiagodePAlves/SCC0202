#ifndef __DICIO_H__
/**
 * Dicionário implementado por uma skip list.
 */
#define __DICIO_H__

#include "utils.h"



// Estrutura do dicionário.
typedef struct dicio dicio_t;

dicio_t *dicio_novo(void);

void dicio_destroi(dicio_t *dicio)
attribute(nonnull);

typedef enum result {
    OK,
    INESPERADO,
    INVALIDA
} result_t;

result_t dicio_insere(dicio_t *restrict dicio, const char *palavra, const char *descricao)
attribute(nonnull);

result_t dicio_altera(dicio_t *restrict dicio, const char *palavra, const char *descricao)
attribute(nonnull);


result_t dicio_remove(dicio_t *restrict dicio, const char *palavra)
attribute(nonnull);

typedef struct const_palavra {
    const char *restrict chave;
    const char *restrict descricao;
} const_palavra_t;

#define palavra_invalida(palavra) \
    ((palavra).chave == NULL || (palavra).descricao == NULL)


const_palavra_t dicio_busca(const dicio_t *restrict dicio, const char *palavra)
attribute(pure, nonnull);

const_palavra_t dicio_lista_por_inicial(const dicio_t *restrict dicio, char inicial);

#endif //__DICIO_H__
