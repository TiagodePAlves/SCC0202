#include "subst.h"
#include "utils.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>


pattern_t read_pattern_line(void) {
    pattern_t pat;

    const char fmt[] = "%"STR(PATTERN_SIZE)"[^\n]\n";
    const int ret = scanf(fmt, pat.str);

    assert_eq(ret, 1);
    return pat;
}

#define MAX_SUB 128



static inline
unsigned substite_same_size(
    char * restrict text, const size_t limit,
    const pattern_t search, const pattern_t subst,
    const size_t subst_size
) {

    unsigned count = 0;

    // search substrings
    char *ptr = text;
    while ((ptr = strstr(ptr, search.str)) != NULL) {
        // function description requires this
        if (count >= MAX_SUB) {
            return count;
        }

        // valid write since 'search' and 'subst' have the same size
        memcpy(ptr, subst.str, subst_size);

        count += 1;
    }

    return count;
}

#define todo() \
    printf("todo!\n"); \
    exit(10)

unsigned pattern_substituition(char *text, const pattern_t search, const pattern_t subst, const size_t max) {
    // sanity check
    assert(text != NULL);

    const size_t search_len = strlen(search.str);
    const size_t subst_len = strlen(subst.str);

    if (search_len < subst_len) {
        todo();
    } else if (search_len > subst_len) {
        todo();
    } else {
        return substite_same_size(text, max, search, subst, subst_len);
    }
}