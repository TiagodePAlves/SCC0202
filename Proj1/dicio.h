#ifndef __DICIO_H__
/**
 * Dicionário implementado por uma skip list.
 */
#define __DICIO_H__

#include "utils.h"


// Estrutura do dicionário.
typedef struct dicio dicio_t;


/**
 * Aloca um novo dicionário.
 *
 * Retorna NULL em erro de alocação.
 */
dicio_t *dicio_novo(void);

/**
 * Desaloca todo o dicionário.
 */
void dicio_destroi(dicio_t *dicio)
attribute(nonnull);


/**
 * Resultado de algumas operações no dicionário.
 */
typedef enum result {
    // Sem erros.
    OK,
    // Operação é inválida, impossível de ser executada.
    INVALIDA,
    // Erro insperado ocorreu durante a operação, deve tentar novamente.
    INESPERADO
} result_t;

/**
 * Insere nova palavra no dicinário, com uma descrição.
 *
 * A operação é inválida se a palavra já existir no dicionário.
 */
result_t dicio_insere(dicio_t *restrict dicio, const char *palavra, const char *descricao)
attribute(nonnull);

/**
 * Altera a descrição de uma palavra do dicionário.
 *
 * A operação é inválida se a palavra não existir no dicionário.
 */
result_t dicio_altera(dicio_t *restrict dicio, const char *palavra, const char *descricao)
attribute(nonnull);

/**
 * Remove uma palavra do dicionário.
 *
 * A operação é inválida se a palavra não existir no dicionário.
 */
result_t dicio_remove(dicio_t *restrict dicio, const char *palavra)
attribute(nonnull);


/**
 * Estrutura de visualização de uma entrada do dicionário,
 * retornadas nas operações de busca.
 */
typedef struct entrada {
    // Palavra ou verbete.
    const char *restrict palavra;
    // Descrição da palavra.
    const char *restrict descricao;
} entrada_t;

/**
 * Checagem de entradas inválidas retornadas de operações.
 */
#define entrada_invalida(entrada) \
    ((entrada).palavra == NULL || (entrada).descricao == NULL)


/**
 * Busca uma entrada do dicionário a partir de seu verbete.
 *
 * Retorna uma entrada inválida caso a palavra não exista no
 * dicionário.
 */
entrada_t dicio_busca(const dicio_t *restrict dicio, const char *palavra)
attribute(pure, nonnull);

/**
 * Lista todas as palavras com um caracter inicial presentes
 * no dicionário, uma a uma.
 *
 * Funciona de forma similar a função `strtok`, em que a
 * chamada com `dicio != NULL` inicia a iteração e as
 * chamadas seguintes, recebendo `NULL` como argumento
 * continua a iteração.
 *
 * Retorna uma entrada inválida quando encerra a iteração.
 */
entrada_t dicio_lista_por_inicial(const dicio_t *restrict dicio, char inicial);

#endif //__DICIO_H__
