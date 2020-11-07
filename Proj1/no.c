#include "no.h"
/* Funções gerais e de inicialização de nós. */


/* * * * * * * * *
 * INICIALIZAÇÃO */

static inline attribute(nonnull)
/**
 * Inicializa o campo `palavra.chave` do `no`.
 * Retorna se a operação obteve sucesso.
 */
bool no_init_chave(no_t *restrict no, const char *restrict chave) {
    // Aloca a chave completa
    char *nova = strdup(chave);
    if unlikely(nova == NULL) {
        return false;
    }
    no->palavra.chave = nova;
    // Guarda as iniciais no padding.
    no->ini = nova[0];
    return true;
}

static inline attribute(nonnull)
/**
 * Inicializa o campo `palavra.descricao` do `no`.
 * Retorna se a operação obteve sucesso.
 */
bool no_init_descr(no_t *restrict no, const char *restrict descricao) {
    // sempre aloca a descrição inteira em uma nova região
    char *nova = strdup(descricao);
    if unlikely(nova == NULL) {
        return false;
    }
    no->palavra.descricao = nova;

    return true;
}

static inline
/**
 * Aloca nó com níveis aleatórios, seguindo
 * a distribuição de `rand_hl`.
 *
 * Todos os campos além de `nivel` são zerados.
 * Retorna NULL em caso de erro.
 */
no_t *no_alloc(void) {
    // escolha aleatória do nível
    uint8_t nivel = rand_hl() + 1;

    // parte fixa da struct
    size_t fixo = sizeof(no_t); // offsetof(no_t, prox);
    // parte flexível
    size_t prox = ((size_t) nivel) * sizeof(no_t *);

    // aloca zerado, fazendo `prox` com NULLs
    no_t *novo = calloc(1, fixo + prox);
    if unlikely(novo == NULL) {
        return NULL;
    }
    // e marca o nível
    novo->nivel = nivel;
    return novo;
}

static inline attribute(nonnull)
/**
 * Aloca nó com a chave e descrição dadas e quantidade de
 * níveis aleatórios, apontando para `NULL`.
 *
 * Retorna NULL em caso de erro.
 */
no_t *no_novo(const char *chave, const char *descricao) {
    // aloca nó com nível aleatório
    no_t *novo = no_alloc();
    if unlikely(novo == NULL) {
        return NULL;
    }
    // inicializa descricao
    if unlikely(!no_init_descr(novo, descricao)) {
        free(novo);
        return NULL;
    }
    // inicializa chave
    if unlikely(!no_init_chave(novo, chave)) {
        free(novo->palavra.descricao);
        free(novo);
        return NULL;
    }
    return novo;
}


/* * * * * * * * * * *
 * OUTRAS OPERAÇÕES  */

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

