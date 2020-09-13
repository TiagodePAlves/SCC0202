#ifndef __UTILS_H__
/* Shared utility macros. */
#define __UTILS_H__

#include <assert.h>
#include <stdint.h>


// https://gcc.gnu.org/onlinedocs/gcc-4.8.5/cpp/Stringification.html
#define __STRINGIFY(token) #token
/* Turns a token or macro into literal string. */
#define STR(token) __STRINGIFY(token)


#ifndef NDEBUG
/* Assertion for variable values on debug builds. On non-debug builds
 * this casts RHS as `void`, to mark variables as unused.
 */
#define assert_eq(variable, expected) \
    assert((variable) == (expected))
/* Assertion for a variable on debug builds, which evaluates to `expr` on debug.
 * On non-debug builds this casts variable as `void`, to mark as unused.
 */
#define assert_expr(variable, expr) \
    assert((expr))
#else
// Change behaviour on non-debug builds to avoid unused errors
#define assert_eq(variable, expected) \
    (void) variable
#define assert_expr(variable) \
    (void) variable
#endif


/* Compiler attributes declaration. */
#define attribute(...) __attribute__((__VA_ARGS__))


/* Gets the maximum of two integers. */
#define max(x, y) \
    (((x) >= (y))? (x) : (y))

/* Gets the minimum of two integers. */
#define min(x, y) \
    (((x) <= (y))? (x) : (y))


/* Type representing 1 byte of data. */
typedef uint8_t byte_t;


// branch hints
/* Indicates that braching is likely to happen. */
#define likely(x)   __builtin_expect((x), 1)
/* Indicates unlikely branches, useful for error checking. */
#define unlikely(x) __builtin_expect((x), 0)

#endif/*__UTILS_H__*/
