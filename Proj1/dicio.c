#include "dicio.h"
#include <stdlib.h>

#include "no.c"

typedef struct dicio {
    // Ponteiros iniciais da lista
    // em cada nível.
    no_t *ini[MAX_NIVEL];
} dicio_t;


/* * * * * * * * *
 * INICIALIZAÇÃO */

/* Alocação. */
dicio_t *dicio_novo(void) {
    return calloc(1, sizeof(dicio_t));
}

attribute(nonnull)
/* Desalocação. */
void dicio_destroi(dicio_t *dicio) {
    // todos os nós existem no primeiro nível
    for (no_t *no = dicio->ini[0]; no != NULL;) {
        no_t *prox = no->prox[0];
        // basta desalocar esse nível
        no_destroi(no);
        no = prox;
    }
    free(dicio);
}


/* * * * * * * * *
 * BUSCA INTERNA */

// Resultado da busca interna de um nó na lista.
typedef struct busca {
    // Lista com ponteiros para campo `prox' na posição
    // certa do nó anterior onde a chave deveria estar,
    // em cada nível.
    no_t **ant[MAX_NIVEL];
    // No com chave igual a procurada, ou NULL se ela
    // não existir.
    no_t *eq;
} busca_t;

static inline attribute(pure, nonnull)
/**
 * Busca na skip list pelo nó com uma certa palavra.
 *
 * Retorna os ponteiros dos campos certos dos nós anteriores
 * onde a palavra deveria estar. Retorna também qual o nó,
 * se existir, com a palavra exata.
 *
 * A flag `para_se_eq` indica que a busca pode ser encerrada
 * prematuramente caso um nó com a palavra exata for
 * encontrado.
 */
busca_t dicio_busca_no(const dicio_t *restrict dicio, const char *palavra, bool para_se_eq) {
    busca_t busca;
    // inicializa sem nenhum nó exato
    busca.eq = NULL;

    // lista com os próximos nós em cada nível
    const no_t **nos = (const no_t **) dicio->ini;
    // busca do anterior em cada nível, começando
    // pelo mais alto
    for (size_t i = MAX_NIVEL; i > 0; i--) {
        // resultado da comparação, assume o final da
        // lista (NULL) como sempre maior
        int cmp = 1;
        // percorre a lista no nível `i-1` enquanto exisistir próximo nó
        // e ele for menor que a palavra buscada
        while (nos[i-1] != NULL && (cmp = no_cmp(nos[i-1], palavra)) < 0) {
            nos = (const no_t **) nos[i-1]->prox;
        }
        // achado o último nó (a lista de próximos dele) no nível `i-1`
        //  que é menor que a chave
        busca.ant[i-1] = (no_t **) nos + i - 1;

        // se o próximo era igual, marca no reultado
        if unlikely(cmp == 0) {
            busca.eq = (no_t *) nos[i-1];
            // se for o bastante, encerra
            if (para_se_eq) {
                return busca;
            }
        }
    }
    return busca;
}

/* * * * * * * * * * * * * *
 * OPERAÇÕES DO DICIONÁRIO */

attribute(nonnull)
/* Inserção no dicionário. */
result_t dicio_insere(dicio_t *restrict dicio, const char *palavra, const char *descricao) {
    // busca os nós anteriores, mas para se encontrar uma chave igual
    busca_t resultado = dicio_busca_no(dicio, palavra, true);
    // nó deve ser novo
    if unlikely(resultado.eq != NULL) {
        return INVALIDA;
    }

    // aloca nova entrada
    no_t *novo = no_novo(palavra, descricao);
    if unlikely(novo == NULL) {
        return INESPERADO;
    }
    // insere o nó em cada nível da lista,
    // até o máximo do nó
    size_t nivel = novo->nivel;
    for (size_t i = 0; i < nivel; i++) {
        novo->prox[i] = *resultado.ant[i];
        *resultado.ant[i] = novo;
    }
    return OK;
}

attribute(nonnull)
/* Alteração do dicionário. */
result_t dicio_altera(dicio_t *restrict dicio, const char *palavra, const char *descricao) {
    // busca os nós anteriores, mas para se encontrar uma chave igual
    busca_t resultado = dicio_busca_no(dicio, palavra, true);
    // nó deveria já estar na lista
    if unlikely(resultado.eq == NULL) {
        return INVALIDA;
    }
    // altera a descrição
    if unlikely(!no_altera_descricao(resultado.eq, descricao)) {
        return INESPERADO;
    }
    return OK;
}

attribute(nonnull)
/* Remoção do dicionário. */
result_t dicio_remove(dicio_t *restrict dicio, const char *palavra) {
    // busca os nós anteriores
    busca_t resultado = dicio_busca_no(dicio, palavra, false);
    // nó deveria estar na lista
    if unlikely(resultado.eq == NULL) {
        return INVALIDA;
    }
    no_t *no = resultado.eq;

    // remove da lista para cada nível até o do nó
    size_t nivel = no->nivel;
    for (size_t i = 0; i < nivel; i++) {
        *resultado.ant[i] = no->prox[i];
    }
    // só então desaloca
    no_destroi(no);
    return OK;
}

static inline attribute(const)
/* Palavra não encontrada no dicionário. */
entrada_t palavra_nao_encontrada(void) {
    // apenas monta uma entrada inválida
    return (entrada_t) {
        .palavra = NULL,
        .descricao = NULL
    };
}

attribute(pure, nonnull)
/* Busca no dicionário. */
entrada_t dicio_busca(const dicio_t *restrict dicio, const char *palavra) {
    // busca os nós anteriores, mas para se encontrar uma chave igual
    busca_t resultado = dicio_busca_no(dicio, palavra, true);
    if unlikely(resultado.eq == NULL) {
        return palavra_nao_encontrada();
    }
    // monta a `entrada_t` do nó
    return no_acessa(resultado.eq);
}

/* Lista entradas do dicionário por inicial. */
entrada_t dicio_lista_por_inicial(const dicio_t *restrict dicio, char inicial) {
    // nó atual da iteração
    static const no_t *no = NULL;

    // novo dicionário
    if (dicio != NULL) {
        // refaz a busca nele
        char palavra[] = {inicial, '\0'};
        busca_t resultado = dicio_busca_no(dicio, palavra, false);
        // e acessa o primeiro nó com chave maior ou igual
        // à inicial
        no = *resultado.ant[0];
    }

    // durante a iteração
    if (no != NULL && no->ini == inicial) {
        entrada_t palavra = no_acessa(no);
        // avança o nó
        no = no->prox[0];
        // e retorna o atual
        return palavra;
    // se não tem nó para buscar (fim da lista)
    // ou ele não tem a inicial correta
    } else {
        // encerra a lista para evitar
        // leituras incorretas
        no = NULL;
        // retorna inválido
        return palavra_nao_encontrada();
    }
}
