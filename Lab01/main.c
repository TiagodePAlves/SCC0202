#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static __inline__
int remove_newline(char *str) {
    const size_t len = strlen(str);

    if (len == 0 || str[len-1] != '\n') {
        return EXIT_FAILURE;
    }
    str[len-1] = '\0';

    return EXIT_SUCCESS;
}

#define BUF 1024ULL

int main(void) {
    char line[BUF] = "";

    for (size_t i = 0; fgets(line, BUF, stdin) != NULL; i++) {
        remove_newline(line);

        printf("line[%zu]: %s\n", i, line);
    }

    return EXIT_SUCCESS;
}
