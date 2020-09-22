#include "fila.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Capacidade da primeira alocação da fila.
#define CAP_INICIAL 32

// Acusa erro e aborta (com segurança) o programa.
#define erro(msg) \
    fprintf(stderr,                         \
        "%s: arquivo \"%s\", linha %d\n",  \
        msg, __FILE__, __LINE__);           \
    exit(1)

// Estrutura para a fila.
struct fila {
    size_t cap;
    elem_t *buf;

    size_t ini;
    size_t tam;
};

/** * * * * * * * * * * * * * * * * * * * * * **/
/** Rotinas de criação e destruição da fila.  **/

static inline attribute(const)
// Representação da fila inicial (vazia).
fila_t nova_fila_vazia(void) {
    return (fila_t) {
        .cap = 0,
        .buf = NULL,
        .ini = 0,
        .tam = 0
    };
}

// Aloca nova fila.
fila_t *fila_nova(void) {
    fila_t *nova = malloc(sizeof(struct fila));
    if (nova == NULL) {
        erro("Erro de alocação");
        return NULL;
    }

    *nova = nova_fila_vazia();
    return nova;
}

attribute(nonnull)
// Desaloca a fila.
void fila_destroi(fila_t *fila) {
    if (fila->buf != NULL) {
        free(fila->buf);
    }

    // remonta como vazia para evitar
    // acesso depois do free
    *fila = nova_fila_vazia();
    free(fila);
}

/** * * * * * * * * * * * * * * * **/
/** Rotinas de checagem da fila.  **/

inline attribute(pure, nonnull)
// Teste de fila vazia.
bool fila_vazia(const fila_t *fila) {
    return fila->tam == 0;
}

inline attribute(pure, nonnull)
// Teste de fila cheia.
bool fila_cheia(const fila_t *fila) {
    return fila->tam == fila->cap;
}

/** * * * * * * * * * * * **/
/** Operações com a fila. **/

static inline attribute(nonnull)
// Dobra a capacidade da fila.
//
// Assume que a fila está cheia (`tam == cap`) e
// o ponteiro não pode ser NULL.
void aumenta_cap_fila(fila_t *fila) {
    size_t old_cap = fila->cap;
    // normalmente dobra a capacidade,
    // mas quando vazia, começa com CAP_INICIAL
    size_t new_cap = 2 * old_cap;
    if (new_cap == 0) {
        new_cap = CAP_INICIAL;
    }

    // ajusta a capacidade do buffer
    elem_t *buf = reallocarray(fila->buf, new_cap, sizeof(elem_t));
    if (buf == NULL) {
        erro("Erro de realocação do buffer");
        return;
    }
    fila->buf = buf;
    fila->cap = new_cap;

    // move os elementos iniciais para o final do novo buffer
    memcpy(fila->buf + old_cap, fila->buf, fila->ini);
}


attribute(nonnull)
// Insere no final da fila.
void fila_insere(fila_t *fila, elem_t elem) {
    if (fila_cheia(fila)) {
        aumenta_cap_fila(fila);
    }

    // nova posição no final da fila
    size_t fim = fila->tam % fila->cap;
    fila->buf[fim] = elem;

    fila->tam++;
}

attribute(nonnull)
// Remove do começo da fila.
bool fila_remove(fila_t *restrict fila, elem_t *restrict elem) {
    if (fila_vazia(fila)) {
        return false;
    }

    // remove e avança o começo
    *elem = fila->buf[fila->ini];
    fila->ini = (fila->ini + 1) % fila->cap;

    fila->tam--;
    return true;
}
