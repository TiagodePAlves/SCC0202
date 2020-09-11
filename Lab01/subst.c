#include "subst.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>


pattern_t read_pattern_line(void) {
    pattern_t pat;

    const char fmt[] = "%"STR(PATTERN_SIZE)"[^\n]\n";
    const int ret = scanf(fmt, pat.str);

    assert_eq(ret, 1);
    return pat;
}

unsigned pattern_substituition(char * text, pattern_t search, pattern_t subst) {
    printf("todo!\n");
    exit(1);
}
