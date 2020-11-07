/**
 * SCC0202 - 2020 2S
 *
 * Nome: William Maehara
 * Código: 6792263
 * * * * * * * * * * * */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dicio.h"



static inline
/* Saídas de erro. */
void mostra_erro(result_t resultado) {
    switch (resultado) {
        case INVALIDA:
            (void) printf("OPERACAO INVALIDA\n");
            break;
        case INESPERADO:
            (void) printf("ERRO INESPERADO\n");
            perror("Lab06");
            break;
        case OK:
            break;
    }
}

// As funções de cada operação recebem o dicionário e os textos com que devem
// trabalhar, que podem ser NULL, casos os argumentos não sejam passados
// corretamente na entrada pedrão. Cada uma delas faz a saída necessária, de
// resultado ou de erro.

static inline attribute(nonnull(1))
/* Operação de inserção. */
void insercao(dicio_t *dicionario, const char *verbete, const char *definicao) {
    // poucos argumentos para a operação
    if (verbete == NULL || definicao == NULL) {
        mostra_erro(INVALIDA);
        return;
    }

    result_t resultado = dicio_insere(dicionario, verbete, definicao);
    // problema de execução
    if (resultado != OK) {
        mostra_erro(resultado);
    }
}

static inline attribute(nonnull(1))
/* Operação de inserção. */
void alteracao(dicio_t *dicionario, const char *verbete, const char *definicao) {
    // poucos argumentos para a operação
    if (verbete == NULL || definicao == NULL) {
        mostra_erro(INVALIDA);
        return;
    }

    result_t resultado = dicio_altera(dicionario, verbete, definicao);
    // problema de execução
    if (resultado != OK) {
        mostra_erro(resultado);
    }
}

static inline attribute(nonnull(1))
void remocao(dicio_t *dicionario, const char *verbete) {
    // poucos argumentos para a operação
    if (verbete == NULL) {
        mostra_erro(INVALIDA);
        return;
    }

    result_t resultado = dicio_remove(dicionario, verbete);
    // problema de execução
    if (resultado != OK) {
        mostra_erro(resultado);
    }
}

static inline attribute(nonnull(1))
void busca(const dicio_t *dicionario, const char *verbete) {
    // poucos argumentos para a operação
    if (verbete == NULL) {
        mostra_erro(INVALIDA);
        return;
    }

    entrada_t entrada = dicio_busca(dicionario, verbete);
    // problema de execução
    if unlikely(entrada_invalida(entrada)) {
        mostra_erro(INVALIDA);
        return;
    }
    // resultado da busca
    (void) printf("%s %s\n", entrada.palavra, entrada.descricao);
}

static inline attribute(nonnull(1))
void impressao(const dicio_t *dicionario, const char *verbete) {
    // poucos argumentos para a operação
    if (verbete == NULL) {
        mostra_erro(INVALIDA);
        return;
    }

    char inicial = verbete[0];
    entrada_t entrada = dicio_lista_por_inicial(dicionario, inicial);
    // nenhuma palavra encontrada
    if unlikely(entrada_invalida(entrada)) {
        (void) printf("NAO HA PALAVRAS INICIADAS POR %c\n", inicial);
        return;
    }
    // se existir alguma, continua a iteração
    do {
        // resultado da listagem
        (void) printf("%s %s\n", entrada.palavra, entrada.descricao);

        // avança a iteração
        entrada = dicio_lista_por_inicial(NULL, inicial);
    // até chegar no sentinela
    } while (!entrada_invalida(entrada));
}

// Funções de tratamento da entrada padrão

// Operações possíveis no dicionário.
typedef enum operacao {
    INSERCAO,
    ALTERACAO,
    REMOCAO,
    BUSCA,
    IMPRESSAO,
    // Encerra o progrma.
    SAIR,
    // Operação inválida.
    DESCONHECIDA
} operacao_t;

static inline attribute(pure, nonnull)
// Transforma texto no código da operação, dada pela
// enum `operacao_t`.
operacao_t op_code(const char *op) {
    // lista com os nomes das operações conhecidas
    const char *nome[] = {"insercao", "alteracao", "remocao", "busca", "impressao", "sair"};
    // códigos dessas operações
    const operacao_t code[] = {INSERCAO, ALTERACAO, REMOCAO, BUSCA, IMPRESSAO, SAIR};
    // quantidade de operações conhecidas
    const size_t len = sizeof(code) / sizeof(operacao_t);

    // acha a operação
    for (size_t i = 0; i < len; i++) {
        if (strcmp(op, nome[i]) == 0) {
            // e retorna o código
            return code[i];
        }
    }
    // operação inválida
    return DESCONHECIDA;
}


// Tamanho do buffer de leitura da linha.
#define S_LINHA 250

int main(void) {
    // construção do dicionário
    dicio_t *dicionario = dicio_novo();
    if (dicionario == NULL) {
        mostra_erro(INESPERADO);
        return EXIT_FAILURE;
    }

    // buffer de leitura da entrada padrão
    char linha[S_LINHA];
    // marcador de execução do programa
    bool em_execucao = true;
    // `fgets` retorna NULL em EOF
    while (em_execucao && fgets(linha, S_LINHA, stdin) != NULL) {
        // separa a entrada nos argumentos possíveis
        (void) strtok(linha, " \n\r");
        char *str1 = strtok(NULL, " \n\r");
        char *str2 = strtok(NULL, "\n\r");

        // aplica a operação correta
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
                mostra_erro(INVALIDA);
        }
    }
    // encerra o dicionário
    dicio_destroi(dicionario);
    return EXIT_SUCCESS;
}
