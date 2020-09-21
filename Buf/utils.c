#include "utils.h"
#include <stdlib.h>
#include <stdio.h>


attribute(noinline, noreturn)
void *_alloc_err(const char *arq, const char *func_name, size_t linha, size_t tam) {
    fprintf(stderr,
        "Problema de alocação:\n"
            "\tArquivo:\t%s\n"
            "\tFunção :\t%s\n"
            "\tLinha  :\t%zu\n"
            "\tBuffer :\t%zu bytes\n",
        arq, func_name, linha, tam
    );

    exit(1);
}
