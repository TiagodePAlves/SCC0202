#ifndef __CONSUMIDOR_H__
/* */
#define __CONSUMIDOR_H__

#include "utils.h"
#include "fila.h"

// Dados de um consumidor
typedef const struct consumidor {
    uint8_t idade;
    // Condição agravante
    bool agravante;
    char nome[];
} consumidor_t;

// Quantidade de grupos
#define GRUPOS 4
// Grupo do consumidor
typedef enum grupo {
    // Em ordem crescente de proridade.

    // Consumidor não idoso e sem agravante de saúde
    COMUM               = 0,
    // Consumidor idoso, mas sem agravante
    IDOSA               = 1,
    // Consumidor não idoso, mas com agravante
    AGRAVANTE           = 2,
    // Consumidor idoso e com agravante de saúde
    IDOSA_COM_AGRAVANTE = 3
} grupo_t;


// Criação dos dados de um consumidor.
// Assume que não ocorre erro de alocação.
consumidor_t *consumidor_novo(const char nome[], uint8_t idade, bool agravante);

// Decisão do grupo ao qual o consumidor pertence.
grupo_t consumidor_grupo(const consumidor_t *consumidor)
attribute(pure, nonnull);

// Criação da fila de consumidores.
// Assume que não ocorre erro de alocação.
fila_t *consumidor_fila_nova(void);

// Desaloca memória da fila de consumidores.
void consumidor_fila_destroi(fila_t *fila)
attribute(nonnull);

// Teste de fila vazia.
bool consumidor_fila_vazia(const fila_t *fila)
attribute(pure, nonnull);

void consumidor_fila_insere(fila_t *restrict fila, consumidor_t *restrict novo)
attribute(nonnull);

bool consumidor_fila_remove(fila_t *restrict fila, consumidor_t *restrict *consumidor)
attribute(nonnull);

#endif//__CONSUMIDOR_H__
