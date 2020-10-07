#include "lista.h"
#include <stdlib.h>

// Marcador de dado inválido.
#define INVALIDO -1
// Maior inteiro válido na lista.
#define INT_MAX 0x7FFFFFFFU


// Nó com valor e endereço do próximo.
typedef struct no {
    unsigned dado;
    struct no *prox;
} no_t;

// Lista ligada circular.
struct lista {
    // ponteiro apenas pro final, pois
    // 'fim->prox' é o início
    no_t *fim;
};


// Cria lista nova (vazia)
lista_t *lista_nova(void) {
    lista_t *lista = malloc(sizeof(struct lista));
    // Aborta em caso de erro.
    if (lista == NULL) {
        exit(1);
        return NULL;
    }

    lista->fim = NULL;
    return lista;
}

static
// Destrói a lista nó a nó, se não for vazia.
void destroi_nos(no_t *fim) {
    no_t *no = fim;

    // destrói o nó final e depois o primeiro
    // e assim por diante até chegar em um nó que
    // aponta para o final
    do {
        // acesso do próximo antes do free
        no_t *prox = no->prox;

        // marca nó como inválido para
        // evitar leitura depois do free
        no->dado = (unsigned) INVALIDO;
        no->prox = NULL;
        free(no);

        // muda pro próximo
        no = prox;

    // até voltar pro fim
    } while (no != fim);
}

// Desaloca lista por completo.
void lista_destroi(lista_t *lista) {
    if (lista->fim != NULL) {
        destroi_nos(lista->fim);
    }

    // proteção de read-after-free
    lista->fim = NULL;
    free(lista);
}

// Teste de lista vazia.
bool lista_vazia(const lista_t *lista) {
    return lista->fim == NULL;
}

// Insere elemento no final da lista.
void lista_insere(lista_t *lista, unsigned num) {
    // aborta com inteiros não convertíveis pra 'int'
    if (num > INT_MAX) {
        exit(1);
        return;
    }

    no_t *novo = malloc(sizeof(struct no));
    // aborta em erro de alocação
    if (novo == NULL) {
        exit(1);
        return;
    }
    novo->dado = num;

    if (lista_vazia(lista)) {
        // elemento único,
        // próximo é ele mesmo
        novo->prox = novo;
    } else {
        // senão insere após o último
        novo->prox = lista->fim->prox;
        lista->fim->prox = novo;
    }
    // e marca como novo fim
    lista->fim = novo;
}

// Avança início da lista de forma circular.
void lista_avanca(lista_t *lista) {
    if (! lista_vazia(lista)) {
        // só avançãr a posição final já muda o início
        lista->fim = lista->fim->prox;
    }
}

// Remove último elemento da lista.
int lista_remove(lista_t *lista) {
    if (lista_vazia(lista)) {
        // resultado negativo
        return INVALIDO;
    }

    no_t *fim = lista->fim;
    no_t *ini = fim->prox;

    if (ini != fim) {
        // só esquece o inicial
        fim->prox = ini->prox;
    } else {
        // lista com um elemento
        // vira vazia
        lista->fim = NULL;
    }

    // lê o resultado antes do free
    int num = (int) ini->dado;

    // desaloca e protege contra read-after-free
    ini->dado = (unsigned) INVALIDO;
    ini->prox = NULL;
    free(ini);

    return num;
}
