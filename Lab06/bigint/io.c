/**
 * Leitura e escrita de 'bigint's.
 */
#include "defs.h"


static
// Multiplicação por 10, que é um shift na lista.
void num_mul10(bigint_t *num) {
    num_t *prox = num_alloc(num->numero.digito);
    prox->prox = num->numero.prox;
    num->numero.prox = prox;
    num->numero.digito = 0;
}


static
// Lê a stream até achar um char diferente de
// 'ignorado' e retorna ele.
char pula_char(FILE *stream, char ignorado) {
    char cur;
    while ((cur = fgetc_unlocked(stream)) >= 0) {
        if (cur != ignorado) {
            return cur;
        }
    }
    // erro de leitura
    exit(EXIT_FAILURE);
    return cur;
}

static inline
// Transforma char para dígito BCD da lista.
digito_t to_digito(char digito) {
    return digito - '0';
}

static inline
// Teste se é um dígito ASCII.
bool ehdigito(char c) {
    return '0' <= c && c <= '9';
}

// Leitura do 'bigint'.
bigint_t *bigint_read(FILE *stream) {
    bigint_t *num = bigint_alloc();

    // pula os espaços antes do número
    char letra = pula_char(stream, ' ');
    // análise do possível sinal
    if (letra == '-' || letra == '+') {
        num->neg = (letra == '-');
        letra = fgetc_unlocked(stream);
    }
    // se começa com zero
    if (letra == '0') {
        // ignora todos os zeros iniciais
        letra = pula_char(stream, '0');
    }
    // caso o número seja apenas zeros
    if (!ehdigito(letra)) {
        return num;
    }

    // começo da lista
    num->numero.digito = to_digito(letra);
    // continua a leitura até achar um não dígito
    while (ehdigito(letra = fgetc_unlocked(stream))) {
        // multiplica por 10
        num_mul10(num);
        // e soma (muda) o novo dígito
        num->numero.digito = to_digito(letra);
    }

    return num;
}

static inline
// Teste se um 'bigint' é zero (ignorando sinal).
bool eh_zero(const bigint_t *num) {
    return num->numero.digito == 0 && num->numero.prox == NULL;
}

static
// Realoca o buffer, mas copiando os dados pro final.
// Requer que 'novo_tam >= tam'.
char *realloc_buffer(char *buffer, size_t tam, size_t novo_tam) {
    char *novo = malloc(novo_tam);
    if (novo == NULL) {
        // erro de alocação
        exit(EXIT_FAILURE);
        return NULL;
    }
    // não têm dados pra copiar
    if (buffer == NULL) {
        return novo;
    }

    // onde inicia a cópia no novo buffer
    size_t dif = novo_tam - tam;
    memcpy(novo + dif, buffer, tam);
    // elimina buffer anterior
    free(buffer);
    return novo;
}

static
// Cria uma string com a representação decimal
// da lista de dígitos.
char *to_string(const num_t *num) {
    // capacidade inicial do buffer
    size_t cap = 8192;
    // e tamanho
    size_t tam = 0;
    char *buffer = realloc_buffer(NULL, 0, cap);

    // já escreve o char nulo
    buffer[cap-1] = '\0';
    tam++;

    // escreve os dígitos do final do buffer pro começo
    for (; num != NULL; num = num->prox) {
        // aumenta o buffer se necessário
        if (tam == cap) {
            buffer = realloc_buffer(buffer, cap, 2 * cap);
            cap *= 2;
        }
        // posição nova, a partir do final
        buffer[cap - tam - 1] = num->digito + '0';
        tam++;
    }
    // move os dados (com '\0') pro começo do buffer
    memmove(buffer, buffer + cap - tam, tam);
    return buffer;
}

// Escrita do 'bigint'.
void bigint_show(FILE *stream, const bigint_t *num) {
    // caso especial, para evitar "-0" como saídas
    if (eh_zero(num)) {
        (void) fputc_unlocked('0', stream);
        return;
    }
    // imprime o sinal
    if (num->neg) {
        (void) fputc_unlocked('-', stream);
    }

    // monta e apresenta o texto
    char *texto = to_string(&num->numero);
    (void) fputs(texto, stream);
    // depois descarta o buffer
    free(texto);
}
