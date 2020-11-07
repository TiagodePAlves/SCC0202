#include "no.h"
#include "init.c"
/* Operações gerais com o nó. */


static inline attribute(nonnull)
/**
 * Desaloca memória de apenas um nó,
 * sem importar com os próximos nós.
 */
void no_destroi(no_t *no) {
    free(no->palavra.chave);
    free(no->palavra.descricao);
    free(no);
}

static inline attribute(nonnull)
/**
 * Altera a descrição de um nó.
 *
 * Retorna se a operação obteve sucesso.
 */
bool no_altera_descricao(no_t *no, const char *descricao) {
    char *antiga = no->palavra.descricao;

    if likely(no_init_descr(no, descricao)) {
        free(antiga);
        return true;
    } else {
        return false;
    }
}

static inline attribute(pure, nonnull)
/**
 * Acessa palavra do nó, sem mutabilidade.
 */
const_palavra_t no_acessa(const no_t *no) {
    return (const_palavra_t) {
        .chave = no->palavra.chave,
        .descricao = no->palavra.descricao
    };
}

static inline attribute(pure, nonnull)
/**
 * Compara nó com string.
 */
int no_cmp(const no_t *no, const char *str) {
    return strcmp(no->palavra.chave, str);
}
