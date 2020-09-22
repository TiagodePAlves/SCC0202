#include "consumidor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// `scanf` com retorno checado
#define cscanf(num, ...) \
    if (scanf(__VA_ARGS__) != num) \
        erro("Leitura inválida")

// `printf` com retorno checado
#define cprintf(fmt, ...) \
    if (_check_fmt(fmt, printf(fmt __VA_OPT__(,) __VA_ARGS__))) \
        erro("Escrita inválida")

static inline attribute(pure, nonnull)
// função auxiliar de checagem do retorno
bool _check_fmt(const char *restrict fmt, int ret) {
    if (fmt[0] == '\0') {
        return ret < 0;
    } else {
        return ret <= 0;
    }
}


static inline
consumidor_t *ler_consumidor(void) {
    char nome[1024];
    uint8_t idade;
    bool agravante;
    cscanf(3, "%1023s %hhu %hhu", nome, &idade, &agravante);

    return consumidor_novo(nome, idade, agravante != 0);
}

static inline attribute(nonnull)
void imprime_consumidor(consumidor_t *consumidor) {
    char agravante = consumidor->agravante? '1' : '0';

    cprintf("%s %hhu %c\n", consumidor->nome, consumidor->idade, agravante);
}

int main(void) {
    size_t N;
    cscanf(1, "%zu", &N);

    fila_t *fila = consumidor_fila_nova();

    for (size_t i = 0; i < N; i++) {
        char op[6];
        cscanf(1, "%5s", op);

        if (strcmp(op, "ENTRA") == 0) {
            consumidor_t *novo = ler_consumidor();
            consumidor_fila_insere(fila, novo);

        } else if (strcmp(op, "SAI") == 0) {
            consumidor_t *proximo;
            if (consumidor_fila_remove(fila, &proximo)) {
                imprime_consumidor(proximo);
                free((void *) proximo);
            } else {
                cprintf("FILA VAZIA\n");
            }
        } else {
            erro("Operação inválida");
        }
    }
    consumidor_fila_destroi(fila);

    return EXIT_SUCCESS;
}
