#include "subst.h"
#include "utils.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


pattern_t read_pattern_line(void) {
    pattern_t pat;

    const char fmt[] = "%"STR(PATTERN_SIZE)"[^\n]\n";
    const int ret = scanf(fmt, pat.str);

    assert_eq(ret, 1);
    return pat;
}


unsigned pattern_substituition(char *text, const pattern_t search, const pattern_t subst, const size_t size) {
    // sanity check
    assert(text != NULL);

    const size_t search_len = strlen(search.str);
    const size_t subst_len = strlen(subst.str);
    size_t text_len = strlen(text);
    // sanity check
    assert(text_len <= size);

    unsigned count = 0;
    // search substrings
    char *match = text;
    while ((match = strstr(match, search.str)) != NULL) {

        if (search_len < subst_len) {
            // difference in size when substituted
            const size_t diff = subst_len - search_len;
            // position of the start of the match
            // SAFETY: when nonnull, 'ptr' is greater or equals to 'text'
            const size_t pos = (size_t) match - (size_t) text;

            // number of bytes to copy
            size_t nbytes = text_len - (pos + search_len);
            // do not overflow buffer when moving
            if (text_len + diff > size) {
                nbytes -= text_len + diff - size;
            }
            // move the bytes after the match until the end of the text, plus null byte
            memmove(match + subst_len, match + search_len, nbytes);

            // update text length
            text_len = min(text_len + diff, size);
        }
        // substite matched pattern
        memcpy(match, subst.str, subst_len);

        if (search_len > subst_len) {
            // position of the start of the match
            const size_t pos = (size_t) match - (size_t) text;
            // move the bytes after the match until the end of the buffer, plus null
            memmove(match + subst_len, match + search_len, text_len - (pos + search_len));

            // update text length
            text_len -= search_len - subst_len;
        }

        // start again after the matched substring
        match += subst_len;
        // update null byte
        text[text_len] = '\0';
        // and mark subtituted
        count += 1;
    }

    return count;
}