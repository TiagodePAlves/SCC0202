#include "no.h"
#include "init.c"
/* Operações gerais com o nó. */


static inline attribute(nonnull)
/**
 * Desaloca memória de apenas um nó,
 * sem importar com os próximos.
 */
void no_destroi(no_t *no) {
    // `chave` é NULL quando cabe em `ini`
    if (no->palavra.chave != NULL) {
        free(no->palavra.chave);
    }
    free(no->palavra.descricao);
    free(no);
}

static inline attribute(pure, nonnull)
/**
 * Acessa palavra do nó, sem mutabilidade.
 */
const_palavra_t no_acessa(const no_t *no) {
    // `chave` é NULL
    if (no->palavra.chave == NULL) {
        return (const_palavra_t) {
            // então a chave está em `ini`
            .chave = no->ini,
            .descricao = no->palavra.descricao
        };
    // caso contrário
    } else {
        // é só remontar a `palavra`
        return (const_palavra_t) {
            .chave = no->palavra.chave,
            .descricao = no->palavra.descricao
        };
    }
}

static inline attribute(const)
/**
 * Comparação lexicográfica de dois caracteres.
 */
int no_char_cmp(char a, char b) {
    return  ((int) a) - ((int) b);
}

static inline attribute(nonnull)
/**
 * Comparação dos primeiros `EXTRA_PADDING` caracteres
 * de duas strings.
 *
 * O resultado é salvo em `cmp`.
 *
 * Retorna se a comparação está completa, ou ainda
 * precisa comparar mais caracteres.
 */
bool no_ini_cmp(const char *lhs, const char *rhs, int *cmp) {
    for (unsigned i = 0; i < EXTRA_PADDING; i++) {
        // compara cada caracter
        int ans = no_char_cmp(lhs[i], rhs[i]);
        // completa se alguma delas termina ou se são diferentes
        if (ans != 0 || lhs[i] == '\0' || rhs[i] == '\0') {
            *cmp = ans;
            return true;
        }
    }
    // comparação não foi completa
    *cmp = 0;
    return false;
}

static inline attribute(pure, nonnull)
/**
 * Compara nó com uma string inicial `ini` e, se
 * preciso, com o restante da string `str`.
 */
int no_strini_cmp(const no_t *no, const char *ini, const char *str) {
    int cmp;
    // compara com 'ini' primeiro
    if (!no_ini_cmp(no->ini, ini, &cmp)) {
        // se precisar, compara com o resto da chave
        cmp = strcmp(no->palavra.chave + EXTRA_PADDING, str + EXTRA_PADDING);
    }
    return cmp;
}

static inline attribute(pure, nonnull)
/**
 * Compara nó com string.
 */
int no_str_cmp(const no_t *no, const char *str) {
    return no_strini_cmp(no, str, str);
}

static inline attribute(pure, nonnull)
/**
 * Compara dois nós.
 */
int no_cmp(const no_t *lhs, const no_t *rhs) {
    return no_strini_cmp(lhs, rhs->ini, rhs->palavra.chave);
}
