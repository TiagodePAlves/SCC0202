#include <stdlib.h>
#include <stdio.h>
#include "lista.h"


#define INSERIR 'i'
#define REMOVER 'r'
#define FIM     'f'


void mostra_descricao(descricao_t descr) {
    if (descr.back == SEM_NO_BACK) {
        printf("[%d,%u]", descr.chave, descr.criacao);
    } else {
        printf("[%d,%u,%u]", descr.chave, descr.criacao, descr.back);
    }
}

void mostra_lista(const lista_t *lista) {
    descricao_t descr;
    void *it = lista_iter(lista, NULL, &descr);
    mostra_descricao(descr);

    while (it != NULL) {
        it = lista_iter(lista, it, &descr);
        fputc_unlocked(' ', stdout);
        mostra_descricao(descr);
    };
    fputc_unlocked('\n', stdout);
}


int main(void) {
    lista_t *lista = lista_nova();

    char op;
    while (scanf("%c", &op) > 0 && op != FIM) {

        chave_t chave;
        unsigned back;

        switch (op) {
            case INSERIR:
                scanf("%d %u\n", &chave, &back);
                lista_insere(lista, chave, back);

                // printf("%c %2d %u: ", op, chave, back);
                break;
            case REMOVER:
                scanf("%d\n", &chave);
                lista_remove(lista, chave);

                // printf("%c %2d  : ", op, chave);
                break;
            default:
                exit(EXIT_FAILURE);
                break;
        }
        // mostra_lista(lista);
    }

    if (lista_vazia(lista)) {
        printf("-1\n");
    } else {
        mostra_lista(lista);
    }

    lista_destroi(lista);
    return EXIT_SUCCESS;
}
