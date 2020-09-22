#include "utils.h"
#include <stdlib.h>
#include <stdio.h>


_Noreturn attribute(nonnull, noreturn)
void _error_exit(const char *restrict msg, const char *restrict arquivo, int linha) {

    fprintf(stderr, "%s: arquivo %s, linha %d\n", msg, arquivo, linha);

    exit(EXIT_FAILURE);
}
