#include "consumidor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// `scanf` com retorno checado
#define cscanf(...) \
    if (scanf(__VA_ARGS__) < 0) exit(EXIT_FAILURE)


static inline
// Leitura dos dados do consumidor da entrada padrão.
consumidor_t *ler_consumidor(void) {
    char nome[1024];
    uint8_t idade;
    bool agravante;
    // leitura dos dados
    cscanf("%1023s %hhu %hhu", nome, &idade, &agravante);
    // e construção no heap
    return consumidor_novo(nome, idade, agravante != 0);
}

static inline attribute(nonnull)
// Escrita dos dados do consumidor na saída padrão.
void imprime_consumidor(consumidor_t *consumidor) {
    // evita erros de números fora do 0-1
    char agravante = consumidor->agravante? '1' : '0';
    // e imprime
    printf("%s %hhu %c\n", consumidor->nome, consumidor->idade, agravante);
}

int main(void) {
    size_t N;
    cscanf("%zu", &N);

    // fila de atendimento dos clientes
    fila_t *fila = consumidor_fila_nova();

    for (size_t i = 0; i < N; i++) {
        // operação
        char op[6];
        cscanf("%5s", op);

        // operação de inserção
        if (strcmp(op, "ENTRA") == 0) {
            consumidor_t *novo = ler_consumidor();
            consumidor_fila_insere(fila, novo);

        // operação de remoção
        } else if (strcmp(op, "SAI") == 0) {
            consumidor_t *proximo;
            if (consumidor_fila_remove(fila, &proximo)) {
                imprime_consumidor(proximo);
                free((void *) proximo);

            } else {
                printf("FILA VAZIA\n");
            }
        // outra operação
        } else {
            fprintf(stderr, "Operação inválida\n");
        }
    }
    consumidor_fila_destroi(fila);

    return EXIT_SUCCESS;
}
