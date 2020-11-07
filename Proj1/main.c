#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dicio.h"


#define S_LINHA 250
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

static inline attribute(nonnull(1))
void insercao(dicio_t *dicionario, const char *verbete, const char *definicao) {
    if (verbete == NULL || definicao == NULL) {
        mostra_erro(LEITURA);
        return;
    }

    result_t resultado = dicio_insere(dicionario, verbete, definicao);
    if (resultado != OK) {
        mostra_erro(resultado);
    }
}

static inline attribute(nonnull(1))
void alteracao(dicio_t *dicionario, const char *verbete, const char *definicao) {
    if (verbete == NULL || definicao == NULL) {
        mostra_erro(LEITURA);
        return;
    }

    result_t resultado = dicio_altera(dicionario, verbete, definicao);
    if (resultado != OK) {
        mostra_erro(resultado);
    }
}

static inline attribute(nonnull(1))
void remocao(dicio_t *dicionario, const char *verbete) {
    if (verbete == NULL) {
        mostra_erro(LEITURA);
        return;
    }

    result_t resultado = dicio_remove(dicionario, verbete);
    if (resultado != OK) {
        mostra_erro(resultado);
    }
}

static inline attribute(nonnull(1))
void busca(const dicio_t *dicionario, const char *verbete) {
    if (verbete == NULL) {
        mostra_erro(LEITURA);
        return;
    }

    const_palavra_t entrada = dicio_busca(dicionario, verbete);
    if (entrada.chave == NULL) {
        mostra_erro(INVALIDO);
    } else {
        (void) printf("%s %s\n", entrada.chave, entrada.descricao);
    }
}

static inline
void impressao(const dicio_t *dicionario, const char *verbete) {
    if (verbete == NULL) {
        mostra_erro(LEITURA);
        return;
    }

    (void) dicionario;
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
operacao_t op_code(const char *op) {
    const char *nome[] = {"insercao", "alteracao", "busca", "impressao", "sair"};
    const operacao_t code[] = {INSERCAO, ALTERACAO, BUSCA, IMPRESSAO, SAIR};
    const size_t len = sizeof(code) / sizeof(operacao_t);

    for (size_t i = 0; i < len; i++) {
        if (strcmp(op, nome[i]) == 0) {
            return code[i];
        }
    }
    return DESCONHECIDA;
}

int main(void) {
    dicio_t *dicionario = dicio_novo();
    if (dicionario == NULL) {
        mostra_erro(INESPERADO);
        return EXIT_FAILURE;
    }

    char linha[S_LINHA];
    bool em_execucao = true;
    while (em_execucao && fgets(linha, S_LINHA, stdin) != NULL) {
        (void) strtok(linha, " \n");
        char *str1 = strtok(NULL, " \n");
        char *str2 = strtok(NULL, "\n");

        switch (op_code(linha)) {
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
            case SAIR:
                em_execucao = false;
                break;
            case DESCONHECIDA:
                mostra_erro(INVALIDO);
        }
    }

    dicio_destroi(dicionario);
    return EXIT_SUCCESS;
}
