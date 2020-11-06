#include "defs.h"



static
void mul10(bigint_t *num) {
    num_t *prox = num_alloc(num->numero.digito);
    prox->prox = num->numero.prox;
    num->numero.prox = prox;
    num->numero.digito = 0;
}


static
char pula_char(FILE *stream, char ignorado) {
    char cur;
    while ((cur = fgetc_unlocked(stream)) >= 0) {
        if (cur != ignorado) {
            return cur;
        }
    }

    exit(EXIT_FAILURE);
    return cur;
}

static inline
digito_t to_digito(char digito) {
    return digito - '0';
}

static inline
bool ehdigito(char c) {
    return '0' <= c && c <= '9';
}

bigint_t *bigint_read(FILE *stream) {
    bigint_t *num = bigint_alloc();

    char letra = pula_char(stream, ' ');
    if (letra == '-') {
        num->neg = true;
        letra = fgetc_unlocked(stream);
    }
    if (letra == '0') {
        letra = pula_char(stream, '0');
    }

    num->numero.digito = to_digito(letra);

    while (ehdigito(letra = fgetc_unlocked(stream))) {
        mul10(num);
        num->numero.digito = to_digito(letra);
    }

    return num;
}

static inline
bool eh_zero(const bigint_t *num) {
    return num->numero.digito == 0 && num->numero.prox == NULL;
}

static
char *realloc_buffer(char *buffer, size_t tam, size_t novo_tam) {
    char *novo = malloc(novo_tam);
    if (novo == NULL) {
        exit(EXIT_FAILURE);
        return NULL;
    }
    if (buffer == NULL) {
        return novo;
    }

    size_t dif = novo_tam - tam;
    memcpy(novo + dif, buffer, tam);

    free(buffer);
    return novo;
}

static inline
char *to_string(const num_t *num) {
    size_t cap = 8192;
    size_t tam = 0;
    char *buffer = realloc_buffer(NULL, 0, cap);

    buffer[cap-1] = '\0';
    tam++;

    for (; num != NULL; num = num->prox) {
        if (tam == cap) {
            buffer = realloc_buffer(buffer, cap, 2 * cap);
            cap *= 2;
        }

        buffer[cap - tam - 1] = num->digito + '0';
        tam++;
    }

    memmove(buffer, buffer + cap - tam, tam);
    return buffer;
}

void bigint_show(FILE *stream, const bigint_t *num) {
    if (eh_zero(num)) {
        (void) fputc_unlocked('0', stream);
        return;
    }

    if (num->neg) {
        (void) fputc_unlocked('-', stream);
    }

    char *texto = to_string(&num->numero);
    (void) fputs(texto, stream);
    free(texto);
}
