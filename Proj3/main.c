/**
 * SCC0202 - 2020 2S
 *
 * Nome: William Maehara
 * Código: 6792263
 * * * * * * * * * * * */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "treap.h"


static inline
/**
 * Leitura da entrada padrão, com checagem de erro.
 * Lê um número ou uma string de 128 bytes (com nulo).
 *
 * Encerra o programa em caso de erro.
 */
void read_stdin(bool num, void *data) {
    int rv;
    // leitura de um número
    if (num) {
        rv = scanf("%llu", (unsigned long long *) data);
    // leitura de uma string
    } else {
        rv = scanf("%127s", (char *) data);
    }
    // problemas de leitura
    if (rv < 0) {
        perror("treap");
        exit(EXIT_FAILURE);
    } else if (rv < 1) {
        fprintf(stderr, "treap: entrada inválida\n");
        exit(EXIT_FAILURE);
    }
}

// Código da operação na árvore.
enum opcode {
    INSERCAO,
    REMOCAO,
    BUSCAR,
    IMPRESSAO
};

static inline
/**
 * Leitura de um número da entrada padrão.
 */
uint64_t read_num(void) {
    long long unsigned num;
    read_stdin(true, &num);
    return num;
}

static inline
/**
 * Leitura do operação a ser aplicada na árvore.
 */
enum opcode read_op(void) {
    char str_op[128];
    read_stdin(false, str_op);

    // strings válidas de operação
    const char *op[] = {"insercao", "remocao", "buscar", "impressao"};
    // opcode de cada uma das operações acima
    const enum opcode code[] = {INSERCAO, REMOCAO, BUSCAR, IMPRESSAO};
    const size_t len = sizeof(code) / sizeof(code[0]);
    // compara com cada string
    for (size_t i = 0; i < len; i++) {
        if (strcmp(str_op, op[i]) == 0) {
            return code[i];
        }
    }
    // nenhuma operação encontrada
    exit(EXIT_FAILURE);
    return -1;
}

static inline
/**
 * Leitura da ordem de impressão da árvore.
 */
enum ordem read_ordem(void) {
    char str_op[128];
    read_stdin(false, str_op);

    // strings das ordens válidas
    const char *op[] = {"ordem", "posordem", "preordem", "largura"};
    const enum ordem code[] = {ORDEM, POSORDEM, PREORDEM, LARGURA};
    const size_t len = sizeof(code) / sizeof(code[0]);
    // compara com cada string
    for (size_t i = 0; i < len; i++) {
        if (strcmp(str_op, op[i]) == 0) {
            // retorna o código relacionado
            return code[i];
        }
    }
    // ordem inválida
    exit(EXIT_FAILURE);
    return -1;
}

static inline
/**
 * Imprime chave e prioridade formatada em par.
 */
void imprime_chave(chave_t chave, priority_t prio) {
    unsigned long long a = chave, b = prio;
    printf("(%llu, %llu) ", a, b);
}

int main(void) {
    struct treap arvore = treap_nova();
    // quantidade de operações realizadas
    size_t n = read_num();

    for (size_t i = 0; i < n; i++) {
        uint64_t chave, prio;
        // leitura a aplicação da operação
        switch (read_op()) {
            case INSERCAO:
                // leitura da chave e prioridade
                chave = read_num();
                prio = read_num();
                // inserção da chave
                if (treap_insere(&arvore, chave, prio)) {
                    printf("Elemento ja existente\n");
                }
                break;
            case REMOCAO:
                chave = read_num();
                // remoção da chave
                if (!treap_remove(&arvore, chave)) {
                    printf("Chave nao localizada\n");
                }
                break;
            case BUSCAR:
                chave = read_num();
                // busca da chave
                if (treap_busca(&arvore, chave)) {
                    printf("1\n");
                } else {
                    printf("0\n");
                }
                break;
            case IMPRESSAO:
                // imprime cada nó da árvore
                treap_percorre(&arvore, imprime_chave, read_ordem());
                // termina a linha com os nós
                printf("\n");
                break;
            default:
                // operação inválida
                return EXIT_FAILURE;
        }
    }
    treap_dealloc(&arvore);
    return EXIT_SUCCESS;
}
