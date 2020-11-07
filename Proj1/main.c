#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dicio.h"


#define S_OP    9
#define S_VERBETE   50
#define S_DEFINICAO 140


static inline
void mostra_erro(result_t resultado) {
    switch (resultado) {
        case INVALIDO:
            (void) printf("OPERACAO INVALIDA\n");
            break;
        case INESPERADO:
            (void) printf("ERRO INESPERADO\n");
            perror("Lab06");
            break;
        case LEITURA:
            (void) printf("PROBLEMAS DE LEITURA\n");
            break;
        case OK:
            break;
    }
}

static inline
void insercao(dicio_t *dicionario, const char *verbete, const char *definicao) {
    result_t resultado = dicio_insere(dicionario, verbete, definicao);
    if (resultado != OK) {
        mostra_erro(resultado);
    }
}

static inline
void alteracao(dicio_t *dicionario, const char *verbete, const char *definicao) {
    (void) dicionario;
    (void) verbete;
    (void) definicao;
    (void) printf("*NÃO IMPLEMENTADO\n");
}

static inline
void remocao(dicio_t *dicionario, const char *verbete) {
    result_t resultado = dicio_remove(dicionario, verbete);
    if (resultado != OK) {
        mostra_erro(resultado);
    }
}

static inline
void busca(const dicio_t *dicionario, const char *verbete) {
    const_palavra_t entrada = dicio_busca(dicionario, verbete);
    if (entrada.chave == NULL) {
        mostra_erro(INVALIDO);
    } else {
        (void) printf("%s %s\n", entrada.chave, entrada.descricao);
    }
}

static inline
void impressao(const dicio_t *dicionario, const char *verbete) {
    (void) dicionario;
    (void) verbete;
    (void) printf("*NÃO IMPLEMENTADO\n");
}


typedef enum operacao {
    INSERCAO,
    ALTERACAO,
    REMOCAO,
    BUSCA,
    IMPRESSAO,
    SAIR,
    DESCONHECIDA
} operacao_t;

static inline attribute(pure)
operacao_t str_op_code(const char op[]) {
    if (strcmp(op, "insercao") == 0) {
        return INSERCAO;
    } else if (strcmp(op, "alteracao") == 0) {
        return ALTERACAO;
    } else if (strcmp(op, "remocao") == 0) {
        return REMOCAO;
    } else if (strcmp(op, "busca") == 0) {
        return BUSCA;
    } else if (strcmp(op, "impressao") == 0) {
        return IMPRESSAO;
    } else if (strcmp(op, "sair") == 0) {
        return SAIR;
    } else {
        return DESCONHECIDA;
    }
}


int main(void) {
    dicio_t *dicionario = dicio_novo();
    if (dicionario == NULL) {
        mostra_erro(INESPERADO);
        return EXIT_FAILURE;
    }

    char op[S_OP + 1];
    while (scanf("%"STR(S_OP)"s", op) >= 1) {
        operacao_t op_code = str_op_code(op);
        if (op_code == SAIR) {
            break;
        }

        char str1[S_VERBETE + 1] = "";
        if (scanf(" %"STR(S_VERBETE)"s", str1) < 1) {
            mostra_erro(LEITURA);
            continue;
        }

        char str2[S_DEFINICAO + 1] = "";
        if (op_code == INSERCAO || op_code == REMOCAO) {
            int rv = scanf(" %"STR(S_DEFINICAO)"[^\n]", str2);
            if (rv < 1) {
                mostra_erro(LEITURA);
                continue;
            }
        }

        switch (op_code) {
            case INSERCAO:
                insercao(dicionario, str1, str2);
                break;
            case ALTERACAO:
                alteracao(dicionario, str1, str2);
                break;
            case REMOCAO:
                remocao(dicionario, str1);
                break;
            case BUSCA:
                busca(dicionario, str1);
                break;
            case IMPRESSAO:
                impressao(dicionario, str1);
                break;
            default:
                mostra_erro(INVALIDO);
        }
    }

    dicio_destroi(dicionario);
    return EXIT_SUCCESS;
}
