#include "consumidor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static inline
consumidor_t *ler_consumidor(void) {
    char nome[1024];
    uint8_t idade;
    bool agravante;
    scanf("%1023s %hhu %hhu", nome, &idade, &agravante);

    return consumidor_novo(nome, idade, agravante != 0);
}

static inline attribute(nonnull)
void imprime_consumidor(consumidor_t *consumidor) {
    char agravante = consumidor->agravante? '1' : '0';

    printf("%s %hhu %c\n", consumidor->nome, consumidor->idade, agravante);
}

int main(void) {
    size_t N;
    scanf("%zu", &N);

    fila_t *fila = consumidor_fila_nova();

    for (size_t i = 0; i < N; i++) {
        char op[6];
        scanf("%5s", op);

        if (strcmp(op, "ENTRA") == 0) {
            consumidor_t *novo = ler_consumidor();
            consumidor_fila_insere(fila, novo);

        } else if (strcmp(op, "SAI") == 0) {
            consumidor_t *proximo;
            if (consumidor_fila_remove(fila, &proximo)) {
                imprime_consumidor(proximo);
                free((void *) proximo);
            } else {
                printf("FILA VAZIA\n");
            }
        } else {
            fprintf(stderr, "Operação inválida\n");
        }
    }
    consumidor_fila_destroi(fila);

    return EXIT_SUCCESS;
}
