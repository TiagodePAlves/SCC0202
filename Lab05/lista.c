#include "lista.h"
#include <stdlib.h>


// Nó da lista ligada.
typedef struct no {
    /* campos obrigatórios */
    // Valor do nó
    chave_t chave;
    // Tempo em que o nó foi criado.
    tempo_t criacao;
    // Algum nó anterior.
    struct no *back;
    // O nó seguinte na lista.
    struct no *prox;
    /* campo extra */
    // O nó anterior na lista.
    struct no *ant;
} no_t;

struct lista {
    // Primeiro nó da lista.
    no_t *cabeca;
    // Último nó da lista.
    no_t *final;
    // Tempo corrido total.
    tempo_t total;
};


/* * * * * * * * * * */
/* Operações Básicas */

// Aloca lista.
lista_t *lista_nova(void) {
    lista_t *nova = malloc(sizeof(struct lista));
    // aborta em erro de alocação
    if (nova == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }

    nova->cabeca = NULL;
    nova->final = NULL;
    nova->total = 0;
    return nova;
}

attribute(nonnull)
// Desaloca lista e nós.
void lista_destroi(lista_t *lista) {
    for (no_t *ptr = lista->cabeca; ptr != NULL;) {
        no_t *prox = ptr->prox;
        free(ptr);
        ptr = prox;
    }
    free(lista);
}

extern inline attribute(pure, nonnull)
// Testa se a lista é vazia
bool lista_vazia(const lista_t *lista) {
    return lista->cabeca == NULL;
}


/* * * * * * */
/* Inserção  */

static attribute(nonnull)
// Acesso da n-ésima posição (pelo final).
no_t *nth_back(const lista_t *lista, unsigned n) {
    unsigned i = 0;
    for (no_t *ptr = lista->final; ptr != NULL; ptr = ptr->ant) {
        if (i++ >= n) {
            return ptr;
        }
    }
    // se a posição não existe retorna o
    // último (do final, i.e. o primeiro)
    return lista->cabeca;
}

static attribute(nonnull)
// Aloca um novo no, sem inserir na lista.
no_t *novo_no(chave_t chave, tempo_t criacao, no_t *back) {
    no_t *novo = malloc(sizeof(no_t));
    // aborta em erro de alocação
    if (novo == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    novo->chave = chave;
    novo->criacao = criacao;
    novo->back = back;
    novo->prox = novo->ant = NULL;

    return novo;
}

attribute(nonnull)
// Cria e insere nó na lista.
void lista_insere(lista_t *lista, chave_t chave, unsigned back) {
    // acha ponteiro par o backref do novo nó
    no_t *backward = (back == 0)? NULL : nth_back(lista, back-1);
    // aloca o nó e aumenta o tempo
    no_t *novo = novo_no(chave, lista->total++, backward);

    // marca os dois ponteiros numa lista vazia
    if (lista_vazia(lista)) {
        lista->cabeca = novo;
        lista->final = novo;
    // ou ajusta as refs do nó
    // final numa lista não-vazia
    } else {
        novo->ant = lista->final;
        lista->final->prox = novo;
        lista->final = novo;
    }
}


/* * * * * */
/* Remoção */

static attribute(nonnull)
// Acha primeiro nó com a `chave` dada ou nulo.
no_t *acha_prox(lista_t *lista, chave_t chave) {
    for (no_t *ptr = lista->cabeca; ptr != NULL; ptr = ptr->prox) {
        if (ptr->chave == chave) {
            return ptr;
        }
    }
    return NULL;
}

static attribute(nonnull)
// Remove um dado nó da lista, ajustando
// marcadores de início e fim da lista.
no_t *remove_no(lista_t *lista, no_t *no) {
    // nó é o único na lista
    if (no->ant == NULL && no->prox == NULL) {
        lista->cabeca = lista->final = NULL;
    // nó é o primeiro da lista
    } else if (no->ant == NULL) {
        lista->cabeca = no->prox;
        no->prox->ant = NULL;
    // é o último da lista
    } else if (no->prox == NULL) {
        lista->final = no->ant;
        no->ant->prox = NULL;
    // nó intermediário
    } else {
        no->ant->prox = no->prox;
        no->prox->ant = no->ant;
    }

    // retorna o nó que entrou no lugar
    no_t *prox = no->prox;
    return prox;
}

attribute(nonnull)
// Acha e remove nó da lista.
void lista_remove(lista_t *lista, chave_t chave) {
    // acha nó e incrementa tempo
    no_t *no = acha_prox(lista, chave);
    lista->total += 1;
    // nenhum nó removido
    if (no == NULL) {
        return;
    }

    // arruma todas as back refs do próximo nó em diante
    for (no_t *ptr = remove_no(lista, no); ptr != NULL; ptr = ptr->prox) {
        if (ptr->back == no) {
            ptr->back = NULL;
        }
    }
    // e desaloca nó
    free(no);
}


/* * * * * * */
/* Iteração  */

static attribute(pure, nonnull)
// Acha a posição do nó na lista.
unsigned posicao_na_lista(const no_t *no) {
    unsigned pos = 0;

    // percorre de forma reversa
    while (no->ant != NULL) {
        no = no->ant;
        // incrementando o marcador de posição
        pos++;
    }
    return pos;
}

attribute(nonnull(1, 3))
// Iterador da lista.
// `iter` é apenas o nó atual.
void *lista_iter(const lista_t *lista, void *iter, descricao_t *descricao) {
    const no_t *no;
    // iniciando da iteração
    if (iter == NULL) {
        no = lista->cabeca;
    // ou continuando
    } else {
        // com o próximo nó
        no = ((const no_t *) iter)->prox;
    }

    // fim da lista,
    // encerra iteração
    if (no == NULL) {
        return NULL;
    }

    // monta a descrição geral
    descricao->chave = no->chave;
    descricao->criacao = no->criacao;
    // e descrição do backref
    if (no->back == NULL) {
        descricao->back = SEM_NO_BACK;
    } else {
        descricao->back = posicao_na_lista(no->back);
    }

    // retorna o nó, para continuar a iteração
    return (void *) no;
}
