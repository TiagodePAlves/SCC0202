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
void read_stdin(bool num, void *data) {
    int rv = scanf(num? "%llu" : "%127s", data);
    // problemas de leitura
    if (rv < 0) {
        perror("treap");
        exit(EXIT_FAILURE);
    } else if (rv < 1) {
        fprintf(stderr, "treap: entrada inválida\n");
        exit(EXIT_FAILURE);
    }
}

enum opcode {
    INSERCAO,
    REMOCAO,
    BUSCAR,
    IMPRESSAO
};

static inline
/**
 * Leitura de um número da entrada padrão.
 *
 * Encerra o programa em caso de erro.
 */
uint64_t read_num(void) {
    long long unsigned num;
    read_stdin(true, &num);
}

static inline
enum opcode read_op(void) {
    char str_op[128];
    read_stdin(false, str_op);

    const char op[] = {"insercao", "remocao", "buscar", "impressao"};
    const enum opcode code[] = {INSERCAO, REMOCAO, BUSCAR, IMPRESSAO};
    const size_t len = sizeof(code) / sizeof(code[0]);

    for (size_t i = 0; i < len; i++) {
        if (strcmp(str_op, op[i]) == 0) {
            return code[i];
        }
    }
    exit(EXIT_FAILURE);
    return -1;
}

static inline
enum ordem read_ordem(void) {
    char str_op[128];
    read_stdin(false, str_op);

    const char op[] = {"ordem", "posordem", "preordem", "largura"};
    const enum ordem code[] = {ORDEM, POSORDEM, PREORDEM, LARGURA};
    const size_t len = sizeof(code) / sizeof(code[0]);

    for (size_t i = 0; i < len; i++) {
        if (strcmp(str_op, op[i]) == 0) {
            return code[i];
        }
    }
    exit(EXIT_FAILURE);
    return -1;
}

static
void imprime_chave(chave_t chave, priority_t prio) {
    unsigned long long a = chave, b = prio;
    printf("(%llu, %llu) ", a, b);
}

int main(void) {
    size_t n = read_num();
    struct treap arvore = treap_nova();

    for (size_t i = 0; i < n; i++) {
        uint64_t chave, prio;

        switch (read_op()) {
            case INSERCAO:
                chave = read_num();
                prio = read_num();
                if (treap_insere(&arvore, chave, prio)) {
                    printf("Elemento ja existente\n");
                }
                break;
            case REMOCAO:
                chave = read_num();
                if (!treap_remove(&arvore, chave)) {
                    printf("Chave nao localizada\n");
                }
                break;
            case BUSCAR:
                chave = read_num();
                if (treap_busca(&arvore, chave)) {
                    printf("1\n");
                } else {
                    printf("0\n");
                }
                break;
            case IMPRESSAO:
                treap_percorre(&arvore, imprime_chave, read_ordem());
                printf("\n");
                break;
            default:
                return EXIT_FAILURE;
        }
    }
    treap_dealloc(&arvore);
    return EXIT_SUCCESS;
}
