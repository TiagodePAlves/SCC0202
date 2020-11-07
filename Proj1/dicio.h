#ifndef __DICIO_H__
#define __DICIO_H__


typedef struct const_palavra {
    const char *restrict chave;
    const char *restrict descricao;
} const_palavra_t;

typedef enum result {
    OK,
    INESPERADO,
    INVALIDO
} result_t;

// Estrutura do dicionário.
typedef struct dicio dicio_t;


dicio_t *dicio_novo(void);


#endif //__DICIO_H__
