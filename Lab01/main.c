#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define BUF 1024ULL

int main(void) {
    char line[BUF] = "";

    for (size_t i = 0; scanf("%1024[^\n]\n", line) == 1; i++) {

        printf("line[%zu]: %s\n", i, line);
    }

    return EXIT_SUCCESS;
}
