#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "subst.h"


#define BUF_SIZE 100

int main(void) {
    char line[BUF_SIZE + 1] = "";

    const char fmt[] = "%"STR(BUF_SIZE)"[^\n]\n";
    while (scanf(fmt, line) == 1) {

        const pattern_t pat = read_pattern_line();
        const pattern_t sub = read_pattern_line();

        pattern_substituition(line, pat, sub, BUF_SIZE + 1);

        printf("%s\n", line);
    }

    return EXIT_SUCCESS;
}
