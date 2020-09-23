#include "consumidor.h"
#include <stdlib.h>
#include <string.h>
#include "fila.h"


static inline attribute(const)
// Tamanho em bytes para representar um consumidor com nome 'tam_nome' caracteres.
size_t tam_bytes_consumidor(size_t tam_nome) {
// https://en.wikipedia.org/wiki/Flexible_array_member#Effect_on_struct_size_and_padding
#ifdef offsetof // de <stddef.h>
    size_t fixo = offsetof(consumidor_t, nome);
#else   // pode conter bytes de padding adicionais
    size_t fixo = sizeof(consumidor_t);
#endif

    return fixo + tam_nome + sizeof((char) '\0');
}

// Criação dos dados de um consumidor.
consumidor_t *consumidor_novo(const char nome[], uint8_t idade, bool agravante) {
    size_t len = strlen(nome);

    struct consumidor *novo = malloc(tam_bytes_consumidor(len));
    // assume que não ocorre problema de alocação

    novo->idade = idade;
    novo->agravante = agravante;
    memcpy(novo->nome, nome, len + 1);

    return novo;
}

attribute(pure, nonnull)
// Decisão do grupo ao qual o consumidor pertence.
grupo_t consumidor_grupo(consumidor_t *consumidor) {
    // ternário não é necessário aqui, mas é mais explicito
    grupo_t idosa = (consumidor->idade >= 60)? IDOSA : COMUM;
    // poderia ser 'agravante << 1';
    grupo_t agravante = consumidor->agravante? AGRAVANTE : COMUM;

    return idosa | agravante;
}


// Criação da fila de consumidores.
fila_t *consumidor_fila_nova(void) {
    return fila_vetor_novo(GRUPOS);
}

attribute(nonnull)
// Desaloca memória da fila de consumidores.
void consumidor_fila_destroi(fila_t *fila) {
    for (size_t i = 0; i < GRUPOS; i++) {
        fila_t *cur = fila_vetor_pos(fila, i);
        fila_destroi(cur, free);
    }
    free(fila);
}


attribute(pure, nonnull)
// Teste de fila vazia.
bool consumidor_fila_vazia(const fila_t *fila) {
    for (size_t i = 0; i < GRUPOS; i++) {
        const fila_t *cur = fila_vetor_pos(fila, i);

        if (! fila_vazia(cur)) {
            return false;
        }
    }
    return true;
}

attribute(nonnull)
void consumidor_fila_insere(fila_t *restrict fila, consumidor_t *restrict novo) {
    grupo_t grp = consumidor_grupo(novo);
    fila_t *cur = fila_vetor_pos(fila, (size_t) grp);
    fila_insere(cur, (void *) novo);
}

attribute(nonnull)
bool consumidor_fila_remove(fila_t *restrict fila, consumidor_t *restrict *consumidor) {
    for (size_t grp = GRUPOS; grp > 0; grp--) {
        fila_t *cur = fila_vetor_pos(fila, grp-1);

        if (fila_remove(cur, (void **) consumidor)) {
            return true;
        }
    }
    return false;
}
