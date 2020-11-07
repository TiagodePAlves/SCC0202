#include "no.h"
/* Funções de inicialização do nó. */


static inline attribute(nonnull)
/**
 * Copia (alocando nova) string de tamanho conhecido.
 * Retorna NULL em erro de alocação.
 */
char *no_str_copy_len(const char *string, size_t len) {
    // aloca espaço para o '\0' também
    char *nova = malloc(len + 1);
    if unlikely(nova == NULL) {
        return NULL;
    }
    // copia conteudo quando alocação tem sucesso
    return memcpy(nova, string, len + 1);
}


static inline attribute(nonnull)
/**
 * Inicializa o campo `palavra.chave` do `no`.
 * Retorna se a operação obteve sucesso.
 */
bool no_init_chave(no_t *restrict no, const char *restrict chave) {
    size_t len = strlen(chave);

    // Se a string cabe no padding
    if (len < EXTRA_PADDING) {
        // não aloca nova chave
        no->palavra.chave = NULL;
        memcpy(no->ini, chave, len + 1);
    // Senão, faz o processo normal
    } else {
        // Aloca a chave completa
        char *nova = no_str_copy_len(chave, len);
        if unlikely(no->palavra.chave == NULL) {
            return false;
        }
        no->palavra.chave = nova;
        // Guarda as iniciais no padding.
        memcpy(no->ini, nova, EXTRA_PADDING);
    }

    return true;
}

static inline attribute(nonnull)
/**
 * Inicializa o campo `palavra.descricao` do `no`.
 * Retorna se a operação obteve sucesso.
 */
bool no_init_descr(no_t *restrict no, const char *restrict descricao) {
    size_t len = strlen(descricao);

    // sempre aloca a descrição inteira em uma nova região
    char *nova = no_str_copy_len(descricao, len);
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
 * Todos os valores além de `nivel` são zerados.
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
        // `no_destroi` é valido, mesmo sem
        // inicializar a chave
        no_destroi(novo);
        return NULL;
    }
    return novo;
}

static inline attribute(nonnull)
/**
 * Altera a descrição de um nó.
 *
 * Retorna se a operação obteve sucesso.
 */
bool no_altera_descricao(no_t *no, const char *descricao) {
    char *nova = no_str_copy_len(descricao, strlen(descricao));
    if (nova == NULL) {
        return false;
    }

    free(no->palavra.descricao);
    no->palavra.descricao = nova;
    return true;
}
