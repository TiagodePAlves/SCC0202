#include "defs.h"

// <ctype.h>
#define isblank(c) \
    ((c) == ' ' || (c) == '\t')

#define isdigit(c) \
    ('0' <= (c) && (c) <= '9')


static
void mul10(bigint_t *num) {
    num_t *prox = num_alloc(num->numero.digito);
    num->numero.prox = prox;
    num->numero.digito = 0;
}


static
char pula_espacos(FILE *stream) {
    char cur;
    while ((cur = fgetc_unlocked(stream)) >= 0) {
        if (!isblank(cur)) {
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

bigint_t *bigint_read(FILE *stream) {
    bigint_t *num = bigint_alloc();

    char letra = pula_espacos(stream);
    if (letra == '-') {
        num->neg = true;
        letra = fgetc_unlocked(stream);
    }

    num->numero.digito = to_digito(letra);

    while (isdigit(letra = fgetc_unlocked(stream))) {
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
    size_t cap = 8192, tam = 0;
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
