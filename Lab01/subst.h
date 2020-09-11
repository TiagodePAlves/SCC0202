#ifndef __SUBST_H__
/* Pattern substituition. */
#define __SUBST_H__

#include <stddef.h>
#include "utils.h"

/* Maximum size of pattern. */
#define PATTERN_SIZE 20

/* Simple patterns for search and substuition. */
typedef struct pattern {
    char str[PATTERN_SIZE + 1];
} pattern_t;


/* Read line from`stdin` as a pattern. */
pattern_t read_pattern_line(void);

/* Subtitutes all matches of `search` by `subst` in `text`.
 * Returns number of times substituted.
 *
 * This is limited to 128 substituitions.
 *
 * At most, `max` characters are written in `text`, plus
 * the null byte.
 */
unsigned pattern_substituition(
    char * text,
    const pattern_t search,
    const pattern_t subst,
    const size_t max
)
attribute(nonnull(1));

#endif/*__SUBST_H__*/
