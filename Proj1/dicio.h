#ifndef __DICIO_H__
#define __DICIO_H__


typedef struct const_palavra {
    const char *restrict chave;
    const char *restrict descricao;
} const_palavra_t;


// Estrutura do dicionário.
typedef struct dicio dicio_t;


dicio_t *dicio_novo(void);


#endif //__DICIO_H__
