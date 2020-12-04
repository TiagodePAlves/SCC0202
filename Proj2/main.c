#include <stdlib.h>
#include <stdio.h>
#include "rbtree.h"

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

static inline
uint32_t read_num(void) {
    long long unsigned num;
    int rv = scanf("%llu", &num);

    if (rv < 0) {
        perror("rbtree");
        exit(EXIT_FAILURE);
    } else if (rv < 1) {
        fprintf(stderr, "rbtree: entrada inválida\n");
        exit(EXIT_FAILURE);
    }
    return (uint32_t) num;
}

static inline
void imprime_chave(chave_t chave, attribute(unused) void *data) {
    printf("%u ", (unsigned) chave);
}

static inline attribute(nonnull)
void aplica_op(struct rbtree *arvore, enum opcode op) {
    bool erro = false;
    chave_t arg = UINT32_MAX;

    switch (op) {
        case INSERCAO:
            erro = rb_insere(arvore, read_num());
            break;
        case SUCESSOR:
            erro = rb_busca_succ(arvore, read_num(), &arg);
            break;
        case PREDECESSOR:
            erro = rb_busca_pred(arvore, read_num(), &arg);
            break;
        case MAXIMO:
            erro = rb_busca_max(arvore, &arg);
            break;
        case MINIMO:
            erro = rb_busca_min(arvore, &arg);
            break;
        case PREORDEM:
            rb_pre_ordem(arvore, imprime_chave, NULL);
            (void) fputc_unlocked('\n', stdout);
            break;
        case EMORDEM:
            rb_em_ordem(arvore, imprime_chave, NULL);
            (void) fputc_unlocked('\n', stdout);
            break;
        case POSORDEM:
            rb_pos_ordem(arvore, imprime_chave, NULL);
            (void) fputc_unlocked('\n', stdout);
            break;
        default:
            erro = true;
            break;
    }

    if (erro) {
        printf("erro\n");
    } else if (arg != UINT32_MAX) {
        printf("%u\n", (unsigned) arg);
    }
}

int main(void) {
    size_t N = read_num();
    struct rbtree arvore = rb_nova();

    for (size_t i = 0; i < N; i++) {
        aplica_op(&arvore, read_num());
    }

    rb_dealloc(&arvore);
    return EXIT_FAILURE;
}
