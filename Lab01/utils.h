#ifndef __UTILS_H__
/* Shared utility macros. */
#define __UTILS_H__

#include <assert.h>


// See https://gcc.gnu.org/onlinedocs/gcc-4.8.5/cpp/Stringification.html
#define __STRINGIFY(token) #token
/* Turns a token or macro into literal string. */
#define STR(token) __STRINGIFY(token)

#ifndef NDEBUG
/* Assertion for variable values on debug builds. On non-debug builds
 * this casts RHS as `void`, to mark variables as unused.
 */
#define assert_eq(variable, expected) \
    assert((variable) == (expected))
#else
// Change behaviour on non-debug builds to avoid unused errors
#define assert_eq(variable, expected) \
    (void) variable
#endif

/* Compiler attributes declaration. */
#define attribute(...) __attribute__((__VA_ARGS__))

#endif/*__UTILS_H__*/
