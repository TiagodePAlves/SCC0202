/*
 * SCC0202 - 2020 2S
 * // TODO
 *
 * Nome: William Maehara
 * Código: 6792263
 */
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"


static
// Simula a roda de Josephus para achar o último.
unsigned josephus(unsigned n, unsigned k) {
    lista_t *roda = lista_nova();
    // inicia a lista com os 'n' participantes
    for (unsigned i = 1; i <= n; i++) {
        lista_insere(roda, i);
    }

    // guarda último removido naquela rodada
    unsigned removido = 0;
    while (! lista_vazia(roda)) {
        // avança 'k - 1' e remove o primeiro
        // é o mesmo que avançar 'k' e remover o último
        for (unsigned i = 1; i < k; i++) {
            lista_avanca(roda);
        }

        // não é inválido porque não é vazia
        removido = (unsigned) lista_remove(roda);
    }

    lista_destroi(roda);
    // retorna o último removido
    return removido;
}


// Entrada do programa.
int main(void) {
    size_t T;
    if (scanf("%zu", &T) != 1) {
        return EXIT_FAILURE;
    }

    for (size_t teste = 1; teste <= T; teste++) {
        unsigned k, n;
        if (scanf("%u %u", &n, &k) != 2) {
            return EXIT_FAILURE;
        }

        unsigned ultimo = josephus(n, k);
        int ret = printf("Caso %zu: %u\n", teste, ultimo);

        if (ret < 0) {
            perror("Lab04");
        }
    }

    return EXIT_SUCCESS;
}
