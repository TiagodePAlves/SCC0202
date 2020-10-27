#include <stdlib.h>
#include <stdio.h>
#include "lista.h"


// Operação de inserção na lista.
#define INSERIR 'i'
// Operação de remoção da lista.
#define REMOVER 'r'
// Encerra as operações.
#define FIM     'f'


// Mostra a descrição do elemento na saída padrão.
void mostra_descricao(descricao_t descr) {
    if (descr.back == SEM_NO_BACK) {
        printf("[%d,%u]", descr.chave, descr.criacao);
    } else {
        printf("[%d,%u,%u]", descr.chave, descr.criacao, descr.back);
    }
}

// Mostra toda a lista na saída padrão.
// Assume uma lista não-vazia.
void mostra_lista(const lista_t *lista) {
    descricao_t descr;
    void *it = lista_iter(lista, NULL, &descr);
    // não checa nulos, por
    // assumir lista não-vazia
    mostra_descricao(descr);

    // percorre imprime o resto dos elementos,
    // com espaço antes
    while ((it = lista_iter(lista, it, &descr)) != NULL) {
        fputc_unlocked(' ', stdout);
        mostra_descricao(descr);
    };
    // fim da lista, fim da linha
    fputc_unlocked('\n', stdout);
}


int main(void) {
    lista_t *lista = lista_nova();

    char op;
    // Laitura até a operação de `FIM`.
    while (scanf("%c", &op) > 0 && op != FIM) {

        // possíveis variáveis usadas
        // na leitura
        chave_t chave;
        unsigned back;

        switch (op) {
            // lê chave e posição de backref para o novo nó
            case INSERIR:
                scanf("%d %u\n", &chave, &back);
                lista_insere(lista, chave, back);
            break;
            // lê a chave e remove o nó com ela
            case REMOVER:
                scanf("%d\n", &chave);
                lista_remove(lista, chave);
            break;
            // aborta em operação desconhecida
            default:
                exit(EXIT_FAILURE);
            break;
        }
    }

    if (lista_vazia(lista)) {
        printf("-1\n");
    } else {
        mostra_lista(lista);
    }

    lista_destroi(lista);
    return EXIT_SUCCESS;
}
