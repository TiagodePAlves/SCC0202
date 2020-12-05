/**
 * SCC0202 - 2020 2S
 *
 * Nome: William Maehara
 * Código: 6792263
 * * * * * * * * * * * */
#include <stdlib.h>
#include <stdio.h>
#include "rbtree.h"

static inline
/**
 * Leitura de um número da entrada padrão.
 *
 * Encerra o programa em caso de erro.
 */
uint32_t read_num(void) {
    long long unsigned num;
    int rv = scanf("%llu", &num);

    // problemas de leitura
    if (rv < 0) {
        perror("rbtree");
        exit(EXIT_FAILURE);
    // entrada não é inteiro
    } else if (rv < 1) {
        fprintf(stderr, "rbtree: entrada inválida\n");
        exit(EXIT_FAILURE);
    }
    return (uint32_t) num;
}

static inline
/**
 * Impressão de uma chave da árvore.
 */
void imprime_chave(chave_t chave) {
    // espaço para separar os números.
    printf("%u ", (unsigned) chave);
}

/**
 * Código das operações da entrada padrão.
 */
enum opcode {
    INSERCAO = 1,
    SUCESSOR = 2,
    PREDECESSOR = 3,
    MAXIMO = 4,
    MINIMO = 5,
    PREORDEM = 6,
    EMORDEM = 7,
    POSORDEM = 8
};

static inline attribute(nonnull)
/**
 * Aplica uma operação na árvore, fazendo a leitura de argumentos
 * quando necessário.
 *
 * Imprime o resultado quando tiver algum ou "erro" se algum
 * problema acontecer.
 */
void aplica_op(struct rbtree *arvore, enum opcode op) {
    // ocorrência de um erro
    bool erro = false;
    // resultado da operação (MAX marca op sem resultado)
    chave_t resultado = UINT32_MAX;

    switch (op) {
        case INSERCAO:
            erro = rb_insere(arvore, read_num());
            break;
        case SUCESSOR:
            erro = rb_busca_succ(arvore, read_num(), &resultado);
            break;
        case PREDECESSOR:
            erro = rb_busca_pred(arvore, read_num(), &resultado);
            break;
        case MAXIMO:
            erro = rb_busca_max(arvore, &resultado);
            break;
        case MINIMO:
            erro = rb_busca_min(arvore, &resultado);
            break;
        // nas ops de percurso, imprime quebra de linha no final
        case PREORDEM:
            rb_pre_ordem(arvore, imprime_chave);
            (void) fputc_unlocked('\n', stdout);
            break;
        case EMORDEM:
            rb_em_ordem(arvore, imprime_chave);
            (void) fputc_unlocked('\n', stdout);
            break;
        case POSORDEM:
            rb_pos_ordem(arvore, imprime_chave);
            (void) fputc_unlocked('\n', stdout);
            break;
        default:
            erro = true;
            break;
    }

    if (erro) {
        printf("erro\n");
    // resultado alterado
    } else if (resultado != UINT32_MAX) {
        printf("%u\n", (unsigned) resultado);
    }
}

int main(void) {
    // número de operações
    size_t N = read_num();
    // árvore utilizada
    struct rbtree arvore = rb_nova();

    for (size_t i = 0; i < N; i++) {
        // lê e aplica operação
        aplica_op(&arvore, read_num());
    }

    // desaloca memória da árvore
    rb_dealloc(&arvore);
    return EXIT_SUCCESS;
}
